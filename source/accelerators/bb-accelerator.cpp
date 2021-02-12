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

#include <iostream>
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
		m_roots.push_back(split(objects[i], objects[i + 1], boxes));
	}
	m_roots.push_back(split(objects.back(), numberOfEntities, boxes));
	mergeRoots();
}

float BBAccelerator::intersectNode(BBNode* node, const Ray& ray, size_t ignoreID, EntityIntersection* result) const {
	if (node->isLeaf()) {
		if (m_entities[node->itemIndex]->getID() != ignoreID) {
			float lastT = result->t;
			if (m_entities[node->itemIndex]->intersects(ray, result->t, result) && result->t != lastT) {
				return result->t;
			}
		}
		return -1.0f;
	}
	return m_boundingBoxes[node->itemIndex].intersects(ray, result->t);
}

bool BBAccelerator::intersects(const Ray& ray, size_t ignoreID, EntityIntersection* result) const {
	std::priority_queue<std::pair<float, const BBNode*>, std::vector<std::pair<float, const BBNode*>>,
		std::greater<std::pair<float, const BBNode*>>> tq;
	tq.push(std::make_pair(0.0f, m_root));
	const BBNode* current;
	float t = -1.0f;
	while (true) {
		if (tq.empty()) {
			break;
		}
		current = tq.top().second;
		if (current->isLeaf()) {
			return true;
		}
		tq.pop();

		t = intersectNode(current->left.get(), ray, ignoreID, result);
		if (t >= 0.0f) {
			tq.push(std::make_pair(t, current->left.get()));
		}

		t = intersectNode(current->right.get(), ray, ignoreID, result);
		if (t >= 0.0f) {
			tq.push(std::make_pair(t, current->right.get()));
		}
	}
	return false;
}

bool BBAccelerator::intersectsAny(const Ray& ray, const SurfacePoint& surface, float maxT) const {
	EntityIntersection result;
	result.t = maxT;
	return intersects(ray, surface.meshID, &result);
}
