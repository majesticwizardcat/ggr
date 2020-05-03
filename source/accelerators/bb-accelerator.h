#pragma once

class BBAccelerator;

#include "accelerators/accelerator.h"
#include "accelerators/bounding-box.h"

#include <utility>
#include <memory>

enum class Axis {
	X_AXIS, Y_AXIS, Z_AXIS
};

class BBNode {
public:
	int itemIndex;
	std::unique_ptr<BBNode> left;
	std::unique_ptr<BBNode> right;

	BBNode();
	BBNode(const BBNode& other);
	BBNode(int itemIndex);

	bool isLeaf() const;
};

class BBAccelerator : public Accelerator {
private:
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::vector<BoundingBox> m_boundingBoxes;
	std::unique_ptr<BBNode> m_root;

	int findSplitIndex(int startIndex, int endIndex,
		const std::vector<std::pair<BoundingBox, int>>& boxes, 
		const Axis& axis) const;
	std::unique_ptr<BBNode> split(int startIndex, int endIndex,
		std::vector<std::pair<BoundingBox, int>>& boxes);
	float pointAxisValue(const Point3& p, const Axis& axis) const;
	bool pointCompAxis(const Point3& p0, const Point3& p1, const Axis& axis) const;
	float calculateGapAverage(std::vector<Point3>& midPoints, const Axis& axis) const;
	Axis findSortAxis(const std::vector<std::pair<BoundingBox, int>>& boxes,
		int startIndex, int endIndex) const;
	void intersects(const Ray& ray, int ignoreID, Intersection* result) const;
	float intersectNode(BBNode* node, const Ray& ray, int ignoreID, Intersection* result) const;
	bool intersectEntityAny(BBNode* node, const Ray& ray, int ignoreID, float maxT) const;
	bool intersectBoxAny(BBNode* node, const Ray& ray, float maxT) const;

public:
	BBAccelerator() { }
	BBAccelerator(const BBAccelerator& other);

	void addMesh(const std::vector<std::shared_ptr<Entity>>& mesh);
	void initialize();
	void intersects(const Ray& ray, Intersection* result) const;
	void intersects(const Ray& ray, const SurfacePoint& surface, Intersection* result) const;
	bool intersects(const Ray& ray, const SurfacePoint& surface, float maxT) const;
	std::unique_ptr<Accelerator> clone() const;
};

