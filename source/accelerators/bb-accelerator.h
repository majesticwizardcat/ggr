#pragma once

class BBAccelerator;

#include "accelerators/bounding-box.h"
#include "intersection/entity-intersection.h"

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

	BBNode() : left(nullptr), right(nullptr), itemIndex(0) { }
	BBNode(const BBNode& other) = delete;
	BBNode(int itemIndex);

	bool isLeaf() const;
};

class BBAccelerator {
private:
	const std::unique_ptr<Entity>* m_entities;
	std::vector<BoundingBox> m_boundingBoxes;
	std::vector<std::unique_ptr<BBNode>> m_roots;
	const BBNode* m_root;

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
	bool intersects(const Ray& ray, size_t ignoreID, EntityIntersection* result, bool findAny) const;
	void mergeRoots();

public:
	BBAccelerator() : m_entities(nullptr) { }
	BBAccelerator(const BBAccelerator& other) = delete;

	inline bool intersects(const Ray& ray, EntityIntersection* result) const {
		return intersects(ray, -1, result, false);
	}

	inline bool intersects(const Ray& ray, const SurfacePoint& surface, EntityIntersection* result) const {
		return intersects(ray, surface.meshID, result, false);
	}

	inline  bool intersectsAny(const Ray& ray, const SurfacePoint& surface, float maxT) const {
		EntityIntersection i;
		i.t = maxT;
		return intersects(ray, surface.meshID, &i, true);
	}

	void initialize(const std::unique_ptr<Entity>* entities, size_t numberOfEntites,
		const std::vector<int>& objects);
};

