#pragma once

class BoundingBox;

#include "entities/entity.h"
#include "meshes/triangle.h"
#include "primitives/point.h"

class BoundingBox {
private:
	float intersectSide(float p, float o, float d, float ox, float oy,
		float dx, float dy, float sx, float sy, float ex, float ey,
		float maxT) const;
	void updateIfValid(float t, float* currentT, float* maxT) const;

public:
	Point3 start;
	Point3 end;

	BoundingBox();
	BoundingBox(const BoundingBox& other);
	BoundingBox(const Point3& start, const Point3& end);
	BoundingBox(const Triangle& t);

	void addPoint(const Point3& p);
	void addBoundingBox(const BoundingBox& b);
	inline float intersects(const Ray& ray) const {
		return intersects(ray, std::numeric_limits<float>::max());
	}

	inline bool intersectsAny(const Ray& ray) const {
		return intersectsAny(ray, std::numeric_limits<float>::max());
	}

	float intersects(const Ray& ray, float maxT) const;
	bool intersectsAny(const Ray& ray, float maxT) const;
	Point3 mid() const;

	inline bool isInside(const Point3& p) const {
		return p.x >= start.x && p.x <= end.x
		&& p.y >= start.y && p.y <= end.y
		&& p.z >= start.z && p.z <= end.z;
	}
};
