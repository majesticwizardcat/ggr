#include "primitives/vector.h"
#include "tools/util.h"

#include <iostream>
#include <cmath>
#include <algorithm>

Vector2::Vector2() : Vector2(0.0f) { }
Vector2::Vector2(float x, float y) : x(x), y(y) { }
Vector2::Vector2(const Vector2& other) : Vector2(other.x, other.y) { }
Vector2::Vector2(const Vector4& v) : Vector2(v.x, v.y) { }
Vector2::Vector2(const Point2& p) : Vector2(p.x, p.y) { }
Vector2::Vector2(const Point2& start, const Point2& end) : Vector2(end.x - start.x, end.y - start.y) { }
Vector2::Vector2(float v) : Vector2(v, v) { }

void Vector2::print() const {
	std::cout << "X: " << x << " Y: " << y << std::endl;
}

void Vector2::normalize() {
	float l = length();
	if (l == 0.0f) {
		return;
	}

	x /= l;
	y /= l;
}

void Vector2::clamp() {
	x = util::clamp(x, 0.0f, 1.0f);
	y = util::clamp(y, 0.0f, 1.0f);
}

float Vector2::length() const {
	return std::sqrt(x * x + y * y);
}

float Vector2::lengthSquared() const {
	return x * x + y * y;
}

float Vector2::distance(const Vector2& v) const {
	return (*this - v).length();
}

float Vector2::dot(const Vector2& other) const {
	return x * other.x + y * other.y;
}

float Vector2::avg() const {
	return (x + y) * 0.5f;
}

float Vector2::max() const {
	return std::max(x, y);
}

float Vector2::min() const {
	return std::min(x, y);
}

bool Vector2::isZero() const {
	return util::equals(x, 0.0f) && util::equals(y, 0.0f);
}

bool Vector2::operator==(const Vector2& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y);
}

Vector2 Vector2::unit() const {
	Vector2 u(x, y);
	u.normalize();
	return u;
}

Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2& other) const {
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator*(const Vector2& other) const {
	return Vector2(x * other.x, y * other.y);
}

Vector2 Vector2::operator/(const Vector2& other) const {
	return Vector2(x / other.x, y / other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
	return Vector2(other, *this);
}

Vector2 Vector2::operator*(float scalar) const {
	return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
	return Vector2(y / scalar, y / scalar);
}

Vector2 Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& other) {
	x *= other.x;
	y *= other.y;
	return *this;
}

Vector2 Vector2::operator/=(const Vector2& other) {
	x /= other.x;
	y /= other.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2 Vector2::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2 Vector2::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

Vector3::Vector3() : Vector3(0.0f) { }
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
Vector3::Vector3(const Vector3& other) : Vector3(other.x, other.y, other.z) { }
Vector3::Vector3(const Normal& n) : Vector3(n.x, n.y, n.z) { }
Vector3::Vector3(const Vector4& v) : Vector3(v.x, v.y, v.z) { }
Vector3::Vector3(const Vector2& v) : Vector3(v.x, v.y, 0.0f) { }
Vector3::Vector3(const Point3& p) : Vector3(p.x, p.y, p.z) { }
Vector3::Vector3(const Point3& start, const Point3& end) : Vector3(end.x - start.x,
	end.y - start.y, end.z - start.z) { }
Vector3::Vector3(float v) : Vector3(v, v, v) { }

void Vector3::print() const {
	std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
}

void Vector3::normalize() {
	float l = length();
	if (l == 0.0f) {
		return;
	}

	x /= l;
	y /= l;
	z /= l;
}

void Vector3::clamp() {
	x = util::clamp(x, 0.0f, 1.0f);
	y = util::clamp(y, 0.0f, 1.0f);
	z = util::clamp(z, 0.0f, 1.0f);
}

float Vector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

float Vector3::lengthSquared() const {
	return x * x + y * y + z * z;
}

float Vector3::distance(const Vector3& v) const {
	return (*this - v).length();
}

float Vector3::dot(const Vector3& other) const {
	return x * other.x + y * other.y + z * other.z;
}

float Vector3::avg() const {
	return (x + y + z) / 3.0f;
}

float Vector3::max() const {
	return std::max(x, std::max(y, z));
}

float Vector3::min() const {
	return std::min(x, std::min(y, z));
}

bool Vector3::isZero() const {
	return util::equals(x, 0.0f) && util::equals(y, 0.0f) && util::equals(z, 0.0f);
}

bool Vector3::operator==(const Vector3& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y)
		&& util::equals(z, other.z);
}

bool Vector3::sameHemisphere(const Vector3& v) const {
	return dot(v) > 0;
}

Vector3 Vector3::unit() const {
	Vector3 u(x, y, z);
	u.normalize();
	return u;
}

Vector3 Vector3::cross(const Vector3& other) const {
	return Vector3(y * other.z - z * other.y, 
			z * other.x - x * other.z,
			x * other.y - y * other.x);
}

Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+(const Vector3& other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator*(const Vector3& other) const {
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::operator/(const Vector3& other) const {
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
	return Vector3(other, *this);
}

Vector3 Vector3::operator*(float scalar) const {
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
	return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3& Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3& Vector3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vector4::Vector4() : Vector4(0.0f) { }
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
Vector4::Vector4(const Vector4& other) : Vector4(other.x, other.y, other.z, other.w) { }
Vector4::Vector4(const Point4& p) : Vector4(p.x, p.y, p.z, p.w) { }
Vector4::Vector4(const Point4& start, const Point4& end) : Vector4(end.x - start.x,
	end.y - start.y, end.z - start.z, end.w - start.w) { }
Vector4::Vector4(const Vector2& v) : Vector4(v.x, v.y, 0.0f, 0.0f) { }
Vector4::Vector4(const Vector3& v) : Vector4(v.x, v.y, v.z, 0.0f) { }
Vector4::Vector4(float v) : Vector4(v, v, v, v) { }

void Vector4::print() const {
	std::cout << "X: " << x << " Y: " << y << " Z: " << z << " W: " << w << std::endl;
}

void Vector4::normalize() {
	float l = length();
	if (l == 0.0f) {
		return;
	}

	x /= l;
	y /= l;
	z /= l;
	w /= l;
}

void Vector4::clamp() {
	x = util::clamp(x, 0.0f, 1.0f);
	y = util::clamp(y, 0.0f, 1.0f);
	z = util::clamp(z, 0.0f, 1.0f);
	w = util::clamp(w, 0.0f, 1.0f);
}

float Vector4::length() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::lengthSquared() const {
	return x * x + y * y + z * z + w * w;
}

float Vector4::distance(const Vector4& v) const {
	return (*this - v).length();
}

float Vector4::dot(const Vector4& other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

float Vector4::avg() const {
	return (x + y + z + w) / 4.0f;
}

float Vector4::max() const {
	return std::max(std::max(x, y), std::max(z, w));
}

float Vector4::min() const {
	return std::min(std::min(x, y), std::min(z, w));
}

bool Vector4::isZero() const {
	return util::equals(x, 0.0f) && util::equals(y, 0.0f) && util::equals(z, 0.0f) && util::equals(w, 0.0f);
}

bool Vector4::operator==(const Vector4& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y)
		&& util::equals(z, other.z)
		&& util::equals(w, other.w);
}

Vector4 Vector4::unit() const {
	Vector4 u(x, y, z, w);
	u.normalize();
	return u;
}

Vector4 Vector4::operator-() const {
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+(const Vector4& other) const {
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator*(const Vector4& other) const {
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4 Vector4::operator/(const Vector4& other) const {
	return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const {
	return Vector4(other, *this);
}

Vector4 Vector4::operator*(float scalar) const {
	return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4 Vector4::operator/(float scalar) const {
	return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vector4& Vector4::operator+=(const Vector4& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vector4& Vector4::operator*=(const Vector4& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

Vector4& Vector4::operator/=(const Vector4& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vector4& Vector4::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

Vector4& Vector4::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

Normal::Normal() : Vector3() { }
Normal::Normal(float x, float y, float z) : Vector3(x, y, z) { }
Normal::Normal(const Normal& other) : Vector3(other) { }
Normal::Normal(const Vector3& v) : Vector3(v) { }
Normal::Normal(const Vector4& v) : Vector3(v) { }

void Normal::faceTowards(const Vector3& to) {
	if (dot(to) < 0.0f) {
		flip();
	}
}

void Normal::flip() {
	x = -x;
	y = -y;
	z = -z;
}

Normal& Normal::operator+=(const Normal& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Normal& Normal::operator*=(const Normal& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

Normal& Normal::operator/=(const Normal& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

Normal& Normal::operator-=(const Normal& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Normal& Normal::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Normal& Normal::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

