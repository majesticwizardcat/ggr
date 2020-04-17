#pragma once

#include "primitives/vector.h"
#include "primitives/point.h"

namespace util {
Vector3 minComponentsVector(const Vector3& v0, const Vector3& v1);
Vector3 maxComponentsVector(const Vector3& v0, const Vector3& v1);
Point3 minComponentsPoint(const Point3& p0, const Point3& p1);
Point3 maxComponentsPoint(const Point3& p0, const Point3& p1);
float clamp(float v, float min, float max);
bool equals(float f0, float f1);
bool less(float f0, float f1);
bool greater(float f0, float f1);
}
