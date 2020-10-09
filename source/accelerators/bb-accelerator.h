#pragma once

class BBAccelerator;

#include "accelerators/bounding-box.h"

#include <utility>
#include <memory>

enum class Axis {
	X_AXIS, Y_AXIS, Z_AXIS
};

class BBNode {
public:
	size_t itemIndex;
	std::unique_ptr<BBNode> left;
	std::unique_ptr<BBNode> right;

	BBNode() { }
	BBNode(const BBNode& other) = delete;
	BBNode(int itemIndex);

	bool isLeaf() const;
};

class BBAccelerator {
private:
	const std::unique_ptr<Entity>* m_entities;
	std::vector<BoundingBox> m_boundingBoxes;
	std::unique_ptr<BBNode> m_root;

	int findSplitIndex(size_t startIndex, size_t endIndex,
		const std::vector<std::pair<BoundingBox, size_t>>& boxes, 
		const Axis& axis) const;
	std::unique_ptr<BBNode> split(size_t startIndex, size_t endIndex,
		std::vector<std::pair<BoundingBox, size_t>>& boxes);
	float pointAxisValue(const Point3& p, const Axis& axis) const;
	bool pointCompAxis(const Point3& p0, const Point3& p1, const Axis& axis) const;
	float calculateGapAverage(std::vector<Point3>& midPoints, const Axis& axis) const;
	Axis findSortAxis(const std::vector<std::pair<BoundingBox, size_t>>& boxes,
		size_t startIndex, size_t endIndex) const;
	void intersects(const Ray& ray, size_t ignoreID, Intersection* result) const;
	float intersectNode(BBNode* node, const Ray& ray, size_t ignoreID, Intersection* result) const;
	bool intersectEntityAny(BBNode* node, const Ray& ray, size_t ignoreID, float maxT) const;

public:
	BBAccelerator() : m_entities(nullptr) { }
	BBAccelerator(const BBAccelerator& other) = delete;

	void initialize(const std::unique_ptr<Entity>* entities, size_t numberOfEntites);
	void intersects(const Ray& ray, Intersection* result) const;
	void intersects(const Ray& ray, const SurfacePoint& surface, Intersection* result) const;
	bool intersects(const Ray& ray, const SurfacePoint& surface, float maxT) const;
};

