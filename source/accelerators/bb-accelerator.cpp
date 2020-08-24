#include "accelerators/bb-accelerator.h"

#include <iostream>
#include <algorithm>
#include <queue>
#include <functional>

BBNode::BBNode() : itemIndex(-1) { }
BBNode::BBNode(const BBNode& other) : itemIndex(other.itemIndex) {
	if (other.left) {
		left = std::make_unique<BBNode>(*other.left);
	}
	if (other.right) {
		right = std::make_unique<BBNode>(*other.right);
	}
}

BBNode::BBNode(int itemIndex) : itemIndex(itemIndex) { }

bool BBNode::isLeaf() const {
	return !(left || right);
}

BBAccelerator::BBAccelerator(const BBAccelerator& other) : m_entities(other.m_entities),
	m_boundingBoxes(other.m_boundingBoxes) {
	if (m_root) {
		m_root = std::make_unique<BBNode>(*other.m_root);
	}
}

void BBAccelerator::addMesh(const std::vector<std::shared_ptr<Entity>>& mesh) {
	m_entities.insert(m_entities.end(), mesh.begin(), mesh.end());
}

int BBAccelerator::findSplitIndex(int startIndex, int endIndex,
	const std::vector<std::pair<BoundingBox, int>>& boxes, const Axis& axis) const {
	return (startIndex + endIndex) / 2;
}

std::unique_ptr<BBNode> BBAccelerator::split(int startIndex, int endIndex,
	std::vector<std::pair<BoundingBox, int>>& boxes) {
	std::unique_ptr<BBNode> node = std::make_unique<BBNode>();
	if (endIndex - startIndex == 1) {
		node->itemIndex = boxes[startIndex].second;
		return node;
	}

	Axis axis = findSortAxis(boxes, startIndex, endIndex);
	auto sortAxis = [&](const std::pair<BoundingBox, int>& b0,
		const std::pair<BoundingBox, int>& b1) {
		return pointCompAxis(b0.first.mid(), b1.first.mid(), axis);
	};
	std::sort(boxes.begin() + startIndex, boxes.begin() + endIndex, sortAxis);

	BoundingBox box;
	for (int i = startIndex; i < endIndex; ++i) {
		box.addBoundingBox(boxes[i].first);
	}
	node->itemIndex = m_boundingBoxes.size();
	m_boundingBoxes.push_back(box);

	int splitIndex = findSplitIndex(startIndex, endIndex, boxes, axis);
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

Axis BBAccelerator::findSortAxis(const std::vector<std::pair<BoundingBox, int>>& boxes,
	int startIndex, int endIndex) const {
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

void BBAccelerator::initialize() {
	if (!m_entities.empty()) {
		std::vector<std::pair<BoundingBox, int>> boxes;
		for (int i = 0; i < m_entities.size(); ++i) {
			boxes.push_back(std::make_pair(m_entities[i]->createBoundingBox(), i));
		}
		m_root = split(0, boxes.size(), boxes);
	}
}

float BBAccelerator::intersectNode(BBNode* node, const Ray& ray, int ignoreID, Intersection* result) const {
	if (node->isLeaf()) {
		if (m_entities[node->itemIndex]->getMeshID() != ignoreID) {
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

void BBAccelerator::intersects(const Ray& ray, int ignoreID, Intersection* result) const {
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

bool BBAccelerator::intersectEntityAny(BBNode* node, const Ray& ray, int ignoreID, float maxT) const {
	return m_entities[node->itemIndex]->getMeshID() != ignoreID
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

std::unique_ptr<Accelerator> BBAccelerator::clone() const {
	return std::make_unique<BBAccelerator>(*this);
}

