#pragma once

class Vector2;
class Vector3;
class Vector4;
class Normal;

#include "primitives/point.h"

class Vector2 {
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);
	Vector2(const Vector4& other);
	Vector2(const Point2& p);
	Vector2(const Point2& start, const Point2& end);
	Vector2(float v);

	void print() const;
	void normalize();
	void clamp();

	float length() const;
	float lengthSquared() const;
	float distance(const Vector2& other) const;
	float dot(const Vector2& other) const;
	float avg() const;
	float max() const;
	float min() const;

	bool isZero() const;
	bool operator==(const Vector2& other) const;
	
	Vector2 unit() const;
	Vector2 operator-() const;
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator*(const Vector2& other) const;
	Vector2 operator/(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;

	Vector2 operator+=(const Vector2& other);
	Vector2 operator*=(const Vector2& other);
	Vector2 operator/=(const Vector2& other);
	Vector2 operator-=(const Vector2& other);
	Vector2 operator*=(float scalar);
	Vector2 operator/=(float scalar);
};

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& other);
	Vector3(const Normal& n);
	Vector3(const Vector2& v);
	Vector3(const Vector4& other);
	Vector3(const Point3& p);
	Vector3(const Point3& start, const Point3& end);
	Vector3(float v);

	void print() const;
	void normalize();
	void clamp();

	float length() const;
	float lengthSquared() const;
	float distance(const Vector3& other) const;
	float dot(const Vector3& other) const;
	float avg() const;
	float max() const;
	float min() const;

	bool isZero() const;
	bool operator==(const Vector3& other) const;
	bool sameHemisphere(const Vector3& v) const;

	Vector3 unit() const;
	Vector3 cross(const Vector3& other) const;
	Vector3 operator-() const;
	Vector3 operator+(const Vector3& other) const;
	Vector3 operator*(const Vector3& other) const;
	Vector3 operator/(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator*(float scalar) const;
	Vector3 operator/(float scalar) const;

	Vector3& operator+=(const Vector3& other);
	Vector3& operator*=(const Vector3& other);
	Vector3& operator/=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(float scalar);
	Vector3& operator/=(float scalar);
};

class Vector4 {
public:
	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4& other);
	Vector4(const Point4& p);
	Vector4(const Point4& start, const Point4& end);
	Vector4(const Vector2& v);
	Vector4(const Vector3& v);
	Vector4(float v);

	void print() const;
	void normalize();
	void clamp();

	float length() const;
	float lengthSquared() const;
	float distance(const Vector4& other) const;
	float dot(const Vector4& other) const;
	float avg() const;
	float max() const;
	float min() const;

	bool isZero() const;
	bool operator==(const Vector4& other) const;

	Vector4 unit() const;
	Vector4 operator-() const;
	Vector4 operator+(const Vector4& other) const;
	Vector4 operator*(const Vector4& other) const;
	Vector4 operator/(const Vector4& other) const;
	Vector4 operator-(const Vector4& other) const;
	Vector4 operator*(float scalar) const;
	Vector4 operator/(float scalar) const;

	Vector4& operator+=(const Vector4& other);
	Vector4& operator*=(const Vector4& other);
	Vector4& operator/=(const Vector4& other);
	Vector4& operator-=(const Vector4& other);
	Vector4& operator*=(float scalar);
	Vector4& operator/=(float scalar);
};

class Normal : public Vector3 {
public:
	Normal();
	Normal(float x, float y, float z);
	Normal(const Normal& other);
	Normal(const Vector3& v);
	Normal(const Vector4& v);

	void faceTowards(const Vector3& to);
	void flip();

	Normal& operator+=(const Normal& other);
	Normal& operator*=(const Normal& other);
	Normal& operator/=(const Normal& other);
	Normal& operator-=(const Normal& other);
	Normal& operator*=(float scalar);
	Normal& operator/=(float scalar);
};

