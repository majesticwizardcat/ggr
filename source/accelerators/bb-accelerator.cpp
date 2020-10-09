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

void BBAccelerator::initialize(const std::unique_ptr<Entity>* entities, size_t numberOfEntities) {
	m_entities = entities;
	if (numberOfEntities > 0) {
		std::vector<std::pair<BoundingBox, size_t>> boxes;
		for (int i = 0; i < numberOfEntities; ++i) {
			boxes.push_back(std::make_pair(m_entities[i]->createBoundingBox(), i));
		}
		m_root = split(0, boxes.size(), boxes);
	}
}

float BBAccelerator::intersectNode(BBNode* node, const Ray& ray, size_t ignoreID, Intersection* result) const {
	if (node->isLeaf()) {
		if (m_entities[node->itemIndex]->getID() != ignoreID) {
			float lastT = result->t;
			m_entities[node->itemIndex]->intersects(ray, result->t, result);
			if (result->hit && result->t != lastT) {
				return result->t;
			}
		}
		return -1.0f;
	}
	return m_boundingBoxes[node->itemIndex].intersects(ray, result->t);
}

void BBAccelerator::intersects(const Ray& ray, size_t ignoreID, Intersection* result) const {
	std::priority_queue<std::pair<float, BBNode*>, std::vector<std::pair<float, BBNode*>>,
		std::greater<std::pair<float, BBNode*>>> tq;
	tq.push(std::make_pair(0.0f, m_root.get()));
	BBNode* current;
	float t = -1.0f;
	while (true) {
		if (tq.empty()) {
			break;
		}
		current = tq.top().second;
		if (current->isLeaf()) {
			break;
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
}

void BBAccelerator::intersects(const Ray& ray, Intersection* result) const {
	intersects(ray, -1, result);
}

void BBAccelerator::intersects(const Ray& ray, const SurfacePoint& surface, Intersection* result) const {
	intersects(ray, surface.meshID, result);
}

bool BBAccelerator::intersectEntityAny(BBNode* node, const Ray& ray, size_t ignoreID, float maxT) const {
	return m_entities[node->itemIndex]->getID() != ignoreID
		&& m_entities[node->itemIndex]->intersects(ray, maxT);
}

bool BBAccelerator::intersects(const Ray& ray, const SurfacePoint& surface, float maxT) const {
	std::vector<BBNode*> hits;
	BBNode* current;
	std::pair<bool, bool> res;
	hits.push_back(m_root.get());
	while (true) {
		if (hits.empty()) {
			break;
		}

		current = hits.back();
		hits.pop_back();
		
		if (current->isLeaf() && intersectEntityAny(current, ray,
			surface.meshID, maxT)) {
			return true;
		}

		else {
			if (m_boundingBoxes[current->left->itemIndex].intersectsAny(ray, maxT)) {
				hits.push_back(current->left.get());
			}

			if (m_boundingBoxes[current->right->itemIndex].intersectsAny(ray, maxT)) {
				hits.push_back(current->right.get());
			}
		}
	}
	return false;
}
