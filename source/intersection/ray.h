#pragma once

class Ray;

#include "primitives/vector.h"
#include "primitives/point.h"
#include "primitives/transformation.h"

#include <memory>

class Ray {
private:
	int m_maxIndex;
	Point3 m_raySpaceShear;

public:
	Point3 origin;
	Vector3 direction;
	float weight;

	Point3 dxOrigin;
	Vector3 dxDirection;
	Point3 dyOrigin;
	Vector3 dyDirection;
	bool isCameraRay;

	Ray();
	Ray(const Point3& origin, const Vector3& direction);
	Ray(const Ray& other);
	Ray(const Point3& origin, const Vector3& direction, float weight);

	void createRaySpace();

	bool operator==(const Ray& other) const;

	Point3 point(float t) const;
	Point3 toRaySpace(const Point3& p) const;
};

