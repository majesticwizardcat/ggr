#include "accelerators/bounding-box.h"
#include "tools/util.h"

#include <algorithm>
#include <limits>

BoundingBox::BoundingBox() {
	float max = std::numeric_limits<float>::max();
	float min = std::numeric_limits<float>::min();
	start.x = max;
	start.y = max;
	start.z = max;
	end.x = min;
	end.y = min;
	end.z = min;
}

BoundingBox::BoundingBox(const BoundingBox& other) : BoundingBox(other.start, other.end) { }
BoundingBox::BoundingBox(const Point3& start, const Point3& end) : start(start), end(end) { }
BoundingBox::BoundingBox(const Triangle& t) : BoundingBox() {
	addPoint(t.v0->position);
	addPoint(t.v1->position);
	addPoint(t.v2->position);
}

void BoundingBox::addPoint(const Point3& p) {
	start.x = std::min(start.x, p.x);
	start.y = std::min(start.y, p.y);
	start.z = std::min(start.z, p.z);
	end.x = std::max(end.x, p.x);
	end.y = std::max(end.y, p.y);
	end.z = std::max(end.z, p.z);
}

void BoundingBox::addBoundingBox(const BoundingBox& b) {
	start.x = std::min(start.x, b.start.x);
	start.y = std::min(start.y, b.start.y);
	start.z = std::min(start.z, b.start.z);
	end.x = std::max(end.x, b.end.x);
	end.y = std::max(end.y, b.end.y);
	end.z = std::max(end.z, b.end.z);

}

bool BoundingBox::isInside(const Point3& p) const {
	return p.x >= start.x && p.x <= end.x
	&& p.y >= start.y && p.y <= end.y
	&& p.z >= start.z && p.z <= end.z;
}

float BoundingBox::intersectSide(float p, float o, float d,
	float ox, float oy, float dx, float dy, float sx, float sy,
	float ex, float ey, float maxT) const {
	float t = (p - o) / d;
	if (t < 0 || t > maxT) {
		return -1.0f;
	}
	float x = ox + dx * t;
	float y = oy + dy * t;
	if (x > sx && x < ex && y > sy && y < ey) {
		return t;
	}
	return -1.0f;
}

void BoundingBox::updateIfValid(float t, float* currentT, float* maxT) const {
	if (t >= 0.0f && (*currentT < 0.0f || t < *currentT)) {
		*maxT = t;
		*currentT = t;
	}
}

float BoundingBox::intersects(const Ray& ray) const {
	return intersects(ray, std::numeric_limits<float>::max());
}

float BoundingBox::intersects(const Ray& ray, float maxT) const {
	if (isInside(ray.origin)) {
		return 0.0f;
	}

	float t = -1.0f;
	if (ray.direction.x != 0.0f) {
		updateIfValid(intersectSide(start.x, ray.origin.x, ray.direction.x,
			ray.origin.z, ray.origin.y, ray.direction.z,
			ray.direction.y, start.z, start.y, end.z, end.y, maxT), &t, &maxT);
		updateIfValid(intersectSide(end.x, ray.origin.x, ray.direction.x,
			ray.origin.z, ray.origin.y, ray.direction.z,
			ray.direction.y, start.z, start.y, end.z, end.y, maxT), &t, &maxT);
	}

	if (ray.direction.y != 0.0f) {
		updateIfValid(intersectSide(start.y, ray.origin.y, ray.direction.y,
			ray.origin.x, ray.origin.z, ray.direction.x,
			ray.direction.z, start.x, start.z, end.x, end.z, maxT), &t, &maxT);
		updateIfValid(intersectSide(end.y, ray.origin.y, ray.direction.y,
			ray.origin.x, ray.origin.z, ray.direction.x,
			ray.direction.z, start.x, start.z, end.x, end.z, maxT), &t, &maxT);
	}
	
	if (ray.direction.z != 0.0f) {
		updateIfValid(intersectSide(start.z, ray.origin.z, ray.direction.z,
			ray.origin.x, ray.origin.y, ray.direction.x,
			ray.direction.y, start.x, start.y, end.x, end.y, maxT), &t, &maxT);
		updateIfValid(intersectSide(end.z, ray.origin.z, ray.direction.z,
			ray.origin.x, ray.origin.y, ray.direction.x,
			ray.direction.y, start.x, start.y, end.x, end.y, maxT), &t, &maxT);
	}
	return t;
}

bool BoundingBox::intersectsAny(const Ray& ray) const {
	return intersectsAny(ray, std::numeric_limits<float>::max());
}

bool BoundingBox::intersectsAny(const Ray& ray, float maxT) const {
	if (isInside(ray.origin)) {
		return true;
	}

	if (ray.direction.x != 0.0f) {
		if (intersectSide(start.x, ray.origin.x, ray.direction.x,
			ray.origin.z, ray.origin.y, ray.direction.z,
			ray.direction.y, start.z, start.y, end.z, end.y,
			maxT) >= 0.0f) {
			return true;
		}
		if (intersectSide(end.x, ray.origin.x, ray.direction.x,
			ray.origin.z, ray.origin.y, ray.direction.z,
			ray.direction.y, start.z, start.y, end.z, end.y,
			maxT) >= 0.0f) {
			return true;
		}
	}

	if (ray.direction.y != 0.0f) {
		if (intersectSide(start.y, ray.origin.y, ray.direction.y,
			ray.origin.x, ray.origin.z, ray.direction.x,
			ray.direction.z, start.x, start.z, end.x, end.z,
			maxT) >= 0.0f) {
			return true;
		}
		if (intersectSide(end.y, ray.origin.y, ray.direction.y,
			ray.origin.x, ray.origin.z, ray.direction.x,
			ray.direction.z, start.x, start.z, end.x, end.z,
			maxT) >= 0.0f) {
			return true;
		}
	}
	
	if (ray.direction.z != 0.0f) {
		if (intersectSide(start.z, ray.origin.z, ray.direction.z,
			ray.origin.x, ray.origin.y, ray.direction.x,
			ray.direction.y, start.x, start.y, end.x, end.y,
			maxT) >= 0.0f) {
			return true;
		}
		if (intersectSide(end.z, ray.origin.z, ray.direction.z,
			ray.origin.x, ray.origin.y, ray.direction.x,
			ray.direction.y, start.x, start.y, end.x, end.y,
			maxT) >= 0.0f) {
			return true;
		}
	}
	return false;
}

Point3 BoundingBox::mid() const {
	return start + (end - start) * 0.5f;
}

