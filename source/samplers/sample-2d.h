#pragma once

class Sample2D;

#include "primitives/vector.h"
#include "primitives/point.h"

class Sample2D {
public:
	float u0;
	float u1;

	Sample2D();
	Sample2D(const Sample2D& other);
	Sample2D(float u0, float u1);

	Sample2D operator+(const Sample2D& right) const;
	Sample2D operator-(const Sample2D& right) const;
	Sample2D operator*(const Sample2D& right) const;
	Sample2D operator*(float scalar) const;
	Point2 operator+(const Point2& p) const;
	Point2 operator*(const Point2& p) const;
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
};

