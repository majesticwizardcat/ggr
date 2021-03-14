#include "accelerators/bb-accelerator.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <functional>

BBNode::BBNode(int itemIndex) : itemIndex(itemIndex) { }

bool BBNode::isLeaf() const {
	return !(left || right);
}

int BBAccelerator::findSplitIndex(size_t startIndex, size_t endIndex,
	const std::vector<std::pair<BoundingBox, size_t>>& boxes, const Axis& axis) const {
	return (startIndex + endIndex) / 2;
}

std::unique_ptr<BBNode> BBAccelerator::split(size_t startIndex, size_t endIndex,
	std::vector<std::pair<BoundingBox, size_t>>& boxes) {
	std::unique_ptr<BBNode> node = std::make_unique<BBNode>();
	if (endIndex - startIndex == 1) {
		node->itemIndex = boxes[startIndex].second;
		return node;
	}

	Axis axis = findSortAxis(boxes, startIndex, endIndex);
	auto sortAxis = [&](const std::pair<BoundingBox, size_t>& b0,
		const std::pair<BoundingBox, size_t>& b1) {
		return pointCompAxis(b0.first.mid(), b1.first.mid(), axis);
	};
	std::sort(boxes.begin() + startIndex, boxes.begin() + endIndex, sortAxis);

	BoundingBox box;
	for (size_t i = startIndex; i < endIndex; ++i) {
		box.addBoundingBox(boxes[i].first);
	}
	node->itemIndex = m_boundingBoxes.size();
	m_boundingBoxes.push_back(box);

	size_t splitIndex = findSplitIndex(startIndex, endIndex, boxes, axis);
	node->left = split(startIndex, splitIndex, boxes);
	node->right = split(splitIndex, endIndex, boxes);
	return node;
}

float BBAccelerator::pointAxisValue(const Point3& p, const Axis& axis) const {
	switch(axis) {
	case Axis::X_AXIS:
		return p.x;
	case Axis::Y_AXIS:
		return p.y;
	case Axis::Z_AXIS:
		return p.z;
	};
	std::cout << "Warning: Undefined axis, returning X_AXIS" << std::endl;
	return p.x;
}

bool BBAccelerator::pointCompAxis(const Point3& p0, const Point3& p1, const Axis& axis) const {
	return pointAxisValue(p0, axis) < pointAxisValue(p1, axis);
}

float BBAccelerator::calculateGapAverage(std::vector<Point3>& midPoints, const Axis& axis) const {
	auto compByAxis = [&](const Point3& p0, const Point3& p1) {
		return pointCompAxis(p0, p1, axis);
	};
	std::sort(midPoints.begin(), midPoints.end(), compByAxis);
	return (pointAxisValue(midPoints.back(), axis)
		- pointAxisValue(midPoints[0], axis)) / (midPoints.size() - 1);
}

Axis BBAccelerator::findSortAxis(const std::vector<std::pair<BoundingBox, size_t>>& boxes,
	size_t startIndex, size_t endIndex) const {
	std::vector<Point3> midPoints;
	for (int i = startIndex; i < endIndex; ++i) {
		midPoints.push_back(boxes[i].first.mid());
	}
	float xavg = calculateGapAverage(midPoints, Axis::X_AXIS);
	float yavg = calculateGapAverage(midPoints, Axis::Y_AXIS);
	float zavg = calculateGapAverage(midPoints, Axis::Z_AXIS);
	if (xavg > yavg) {
		if (xavg > zavg) {
			return Axis::X_AXIS;
		}
		return Axis::Z_AXIS;
	}
	if (yavg > zavg) {
		return Axis::Y_AXIS;
	}
	return Axis::Z_AXIS;
}

void BBAccelerator::mergeRoots() {
	auto sortNodes = [&](const std::unique_ptr<BBNode>& n0,
		const std::unique_ptr<BBNode>& n1) {
			return m_boundingBoxes[n0->itemIndex].mid().x <
				m_boundingBoxes[n1->itemIndex].mid().x;
	};
	std::sort(m_roots.begin(), m_roots.end(), sortNodes);
	while (m_roots.size() > 1) {
		int endIndex = m_roots.size() - m_roots.size() % 2;
		for (int i = 0; i < endIndex; i += 2) {
			int nextIndex = m_boundingBoxes.size();
			BoundingBox b;
			b.addBoundingBox(m_boundingBoxes[m_roots[i]->itemIndex]);
			b.addBoundingBox(m_boundingBoxes[m_roots[i + 1]->itemIndex]);
			m_boundingBoxes.push_back(b);
			std::unique_ptr<BBNode> n = std::make_unique<BBNode>();
			n->itemIndex = nextIndex;
			n->left = std::move(m_roots[i]);
			n->right = std::move(m_roots[i + 1]);
			m_roots[i] = std::move(n);
		}
		int pad = 0;
		for (int i = 1; i < endIndex; i += 2) {
			m_roots.erase(m_roots.begin() + i - pad);
			pad++;
		}
	}
	m_root = m_roots[0].get();
}

void BBAccelerator::initialize(const std::unique_ptr<Entity>* entities, size_t numberOfEntities,
		const std::vector<int>& objects) {
	m_entities = entities;
	std::vector<std::pair<BoundingBox, size_t>> boxes;
	for (int i = 0; i < numberOfEntities; ++i) {
		boxes.push_back(std::make_pair(m_entities[i]->createBoundingBox(), i));
	}
	for (int i = 0; i < objects.size() - 1; ++i) {
		if (objects[i + 1] - objects[i] > 0) {
			m_roots.push_back(split(objects[i], objects[i + 1], boxes));
		}
	}
	if (numberOfEntities - objects.back() > 0) {
		m_roots.push_back(split(objects.back(), numberOfEntities, boxes));
	}
	mergeRoots();
}

bool BBAccelerator::intersects(const Ray& ray, size_t ignoreID, EntityIntersection* result, bool findAny) const {
	const BBNode* stack[128];
	int stackBack;
	const BBNode* current;
	bool hit = false;
	const Vector3 invrDir = ONE_VECTOR / ray.direction;
	stack[0] = m_root->left.get();
	stack[1] = m_root->right.get();
	stackBack = 2;
	while (stackBack > 0) {
		current = stack[--stackBack];
		if (current->isLeaf()) {
			if (m_entities[current->itemIndex]->getID() != ignoreID
				&& m_entities[current->itemIndex]->intersects(ray, result)) {
				if (findAny) {
					return true;
				}
				hit = true;
			}
		}
		else if (m_boundingBoxes[current->itemIndex].intersectsAny(ray, invrDir, result->t)) {
			stack[stackBack++] = current->left.get();
			stack[stackBack++] = current->right.get();
		}
	}
	return hit;
}
