#include "samplers/sample-2d.h"

Sample2D::Sample2D() : u0(0.0f), u1(0.0f) { }
Sample2D::Sample2D(const Sample2D& other) : u0(other.u0), u1(other.u1) { }
Sample2D::Sample2D(float u0, float u1) : u0(u0), u1(u1) { }

Sample2D Sample2D::operator+(const Sample2D& right) const {
	return Sample2D(u0 + right.u0, u1 + right.u1);
}

Sample2D Sample2D::operator-(const Sample2D& right) const {
	return Sample2D(u0 - right.u0, u1 - right.u1);
}

Sample2D Sample2D::operator*(const Sample2D& right) const {
	return Sample2D(u0 * right.u0, u1 * right.u1);
}

Sample2D Sample2D::operator*(float scalar) const {
	return Sample2D(u0 * scalar, u1 * scalar);
}

Point2 Sample2D::operator+(const Point2& p) const {
	return Point2(p.x + u0, p.y + u1);
}

Point2 Sample2D::operator*(const Point2& p) const {
	return Point2(p.x * u0, p.y * u1);
}

Vector2 Sample2D::operator+(const Vector2& v) const {
	return Vector2(v.x + u0, v.y + u1);
}

Vector2 Sample2D::operator*(const Vector2& v) const {
	return Vector2(v.x * u0, v.y * u1);
}

