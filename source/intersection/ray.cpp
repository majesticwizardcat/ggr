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
	float absX = std::abs(direction.x);
	float absY = std::abs(direction.y);
	float absZ = std::abs(direction.z);
	if (absZ < absX) {
		if (absX > absY) {
			m_maxIndex = 0;
		}
		else {
			m_maxIndex = 1;
		}
	}
	// absZ > absX
	else {
		if (absZ > absY) {
			m_maxIndex = 2;
		}
		else {
			m_maxIndex = 1;
		}
	}

	// Suggestion 1: return the m_maxIndex value from a procedure
	// Suggestion 2: move each of the following code inside each
	// case.
	// Also note: m_raySpaceShear.z =  oneOverZ; is common in all cases
	if (m_maxIndex == 2) {
		float oneOverZ = 1.0f / direction.z;
		m_raySpaceShear.x = -direction.x * oneOverZ;
		m_raySpaceShear.y = -direction.y * oneOverZ;
		m_raySpaceShear.z =  oneOverZ;
	}
	else if (m_maxIndex == 1) {
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
	if (m_maxIndex == 2) {
		float z = p.z - origin.z;
		return Point3((p.x - origin.x) + m_raySpaceShear.x * z,
			(p.y - origin.y) + m_raySpaceShear.y * z,
			z * m_raySpaceShear.z);
	}

	if (m_maxIndex == 1) {
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

