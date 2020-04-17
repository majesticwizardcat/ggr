#include "primitives/point.h"
#include "tools/util.h"

#include <iostream>

Point2::Point2() : Point2(0.0f) { }
Point2::Point2(float x, float y) : x(x), y(y) { }
Point2::Point2(const Point2& other) : Point2(other.x, other.y) { }
Point2::Point2(const Point4& p) : Point2(p.x, p.y) { }
Point2::Point2(const Vector2& v) : Point2(v.x, v.y) { }
Point2::Point2(float v) : Point2(v, v) { }

void Point2::print() const {
	std::cout << "X: " << x << " Y: " << y;
}

float Point2::distance(const Point2& p) const {
	return (*this - p).length();
}

bool Point2::isZero() const {
	return util::equals(x, 0.0f) && util::equals(y, 0.0f);
}

bool Point2::operator==(const Point2& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y);
}

Point2 Point2::operator+(const Vector2& v) const {
	return Point2(x + v.x, y + v.y);
}

Point2 Point2::operator-(const Vector2& v) const {
	return Point2(x - v.x, y - v.y);
}

Point2 Point2::operator*(float scalar) const {
	return Point2(x * scalar, y * scalar);
}

Point2 Point2::operator/(float scalar) const {
	return Point2(x / scalar, y / scalar);
}

Point2& Point2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

Point2& Point2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

Point2& Point2::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Point2& Point2::operator/= (float scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

Vector2 Point2::operator-(const Point2& right) const {
	return Vector2(x - right.x, y - right.y);
}

Point3::Point3() : Point3(0.0f) { }
Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) { }
Point3::Point3(const Point3& other) : Point3(other.x, other.y, other.z) { }
Point3::Point3(const Point4& p) : Point3(p.x, p.y, p.z) { }
Point3::Point3(const Vector3& v) : Point3(v.x, v.y, v.z) { }
Point3::Point3(const Point2& p) : Point3(p.x, p.y, 0.0f) { }
Point3::Point3(float v) : Point3(v, v, v) { }

void Point3::print() const {
	std::cout << "X: " << x << " Y: " << y << " Z: " << z;
}

float Point3::distance(const Point3& p) const {
	return (*this - p).length();
}

bool Point3::isZero() const {
	return util::equals(x, 0.0f) && util::equals(y, 0.0f) && util::equals(z, 0.0f);
}

bool Point3::operator==(const Point3& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y)
		&& util::equals(z, other.z);
}

Point3 Point3::operator+(const Vector3& v) const {
	return Point3(x + v.x, y + v.y, z + v.z);
}

Point3 Point3::operator-(const Vector3& v) const {
	return Point3(x - v.x, y - v.y, z - v.z);
}

Point3 Point3::operator*(float scalar) const {
	return Point3(x * scalar, y * scalar, z * scalar);
}

Point3 Point3::operator/(float scalar) const {
	return (*this) * (1.0f / scalar);
}

Point3& Point3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Point3& Point3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Point3& Point3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Point3& Point3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vector3 Point3::operator-(const Point3& right) const {
	return Vector3(x - right.x, y - right.y, z - right.z);
}

Point4::Point4() : Point4(0.0f, 0.0f, 0.0f, 1.0f) { }
Point4::Point4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
Point4::Point4(const Point4& other) : Point4(other.x, other.y, other.z, other.w) { }
Point4::Point4(const Vector4& v) : Point4(v.x, v.y, v.z, v.w) { }
Point4::Point4(const Point2& p) : Point4(p.x, p.y, 0.0f, 1.0f) { }
Point4::Point4(const Point3& p) : Point4(p.x, p.y, p.z, 1.0f) { }
Point4::Point4(float v) : Point4(v, v, v, v) { }

void Point4::print() const {
	std::cout << "X: " << x << " Y: " << y << " Z: " << z << " W: ";
}

float Point4::distance(const Point4& p) const {
	return (*this - p).length();
}

bool Point4::isZero() const {
	return util::equals(x, 0.0f) && util::equals(y, 0.0f) && util::equals(z, 0.0f) && util::equals(w, 0.0f);
}

bool Point4::operator==(const Point4& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y)
		&& util::equals(z, other.z)
		&& util::equals(w, other.w);
}

Point4 Point4::operator+(const Vector4& v) const {
	return Point4(x + v.x, y + v.y, z + v.z, w + v.w); 
}

Point4 Point4::operator-(const Vector4& v) const {
	return Point4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Point4 Point4::operator*(float scalar) const {
	return Point4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Point4 Point4::operator/(float scalar) const {
	return (*this) * (1.0f / scalar);
}

Point4& Point4::operator+=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Point4& Point4::operator-=(const Vector4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Point4& Point4::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

Point4& Point4::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

Vector4 Point4::operator-(const Point4& right) const {
	return Vector4(x - right.x, y - right.y, z - right.z, w - right.w);
}

