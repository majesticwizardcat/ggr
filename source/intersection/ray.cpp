#include "intersection/ray.h"

#include <iostream>

Ray::Ray() : isCameraRay(false) { }
Ray::Ray(const Point3& origin, const Vector3& direction) : Ray(origin, direction, 1.0f) { }
Ray::Ray(const Ray& other) : origin(other.origin), direction(other.direction),
	weight(other.weight), m_raySpaceShear(other.m_raySpaceShear), dxOrigin(other.dxOrigin),
	dxDirection(other.dxDirection), dyOrigin(other.dyOrigin), dyDirection(other.dyDirection),
	isCameraRay(other.isCameraRay) { }
Ray::Ray(const Point3& origin, const Vector3& direction, float weight) : origin(origin),
	direction(direction), weight(weight), isCameraRay(false) { }

void Ray::createRaySpace() {
	if (direction.z != 0.0f) {
		float oneOverZ = 1.0f / direction.z;
		m_raySpaceShear.x = -direction.x * oneOverZ;
		m_raySpaceShear.y = -direction.y * oneOverZ;
		m_raySpaceShear.z =  oneOverZ;
	}
	else if (direction.y != 0.0f) {
		float oneOverZ = 1.0f / direction.y;
		m_raySpaceShear.x = -direction.x * oneOverZ;
		m_raySpaceShear.y = -direction.z * oneOverZ;
		m_raySpaceShear.z =  oneOverZ;
	}
	else {
		float oneOverZ = 1.0f / direction.x;
		m_raySpaceShear.x = -direction.z * oneOverZ;
		m_raySpaceShear.y = -direction.y * oneOverZ;
		m_raySpaceShear.z =  oneOverZ;
	}
}

bool Ray::operator==(const Ray& other) const {
	return origin == other.origin
		&& direction == other.direction;
}

Point3 Ray::point(float t) const {
	return origin + direction * t;
}

Point3 Ray::toRaySpace(const Point3& p) const {
	if (direction.z != 0) {
		float z = p.z - origin.z;
		return Point3((p.x - origin.x) + m_raySpaceShear.x * z,
			(p.y - origin.y) + m_raySpaceShear.y * z,
			z * m_raySpaceShear.z);
	}

	if (direction.y != 0) {
		float z = p.y - origin.y;
		return Point3((p.x - origin.x) + m_raySpaceShear.x * z,
			(p.z - origin.z) + m_raySpaceShear.y * z,
			z * m_raySpaceShear.z);
	}

	float z = p.x - origin.x;
	return Point3((p.z - origin.z) + m_raySpaceShear.x * z,
		(p.y - origin.y) + m_raySpaceShear.y * z,
		z * m_raySpaceShear.z);
}

