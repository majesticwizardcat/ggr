#pragma once

class Point2;
class Point3;
class Point4;

#include "primitives/vector.h"

#include <math.h>

class Point2 {
public:
	float x;
	float y;

	Point2();
	Point2(float x, float y);
	Point2(const Point2& other);
	Point2(const Point4& p);
	Point2(const Vector2& v);
	Point2(float v);

	void print() const;

	float distance(const Point2& p) const;

	bool isZero() const;
	bool operator==(const Point2& other) const;

	Point2 operator+(const Vector2& v) const;
	Point2 operator-(const Vector2& v) const;
	Point2 operator*(float scalar) const;
	Point2 operator/(float scalar) const;

	Point2& operator+=(const Vector2& v);
	Point2& operator-=(const Vector2& v);
	Point2& operator*=(float scalar);
	Point2& operator/=(float scalar);

	Vector2 operator-(const Point2& right) const;
};

class Point3 {
public:
	float x;
	float y;
	float z;

	Point3();
	Point3(float x, float y, float z);
	Point3(const Point3& other);
	Point3(const Point4& p);
	Point3(const Vector3& v);
	Point3(const Point2& p);
	Point3(float v);

	void print() const;

	float distance(const Point3& p) const;

	bool isZero() const;
	bool operator==(const Point3& other) const;

	Point3 operator+(const Vector3& v) const;
	Point3 operator-(const Vector3& v) const;
	Point3 operator*(float scalar) const;
	Point3 operator/(float scalar) const;

	Point3& operator+=(const Vector3& v);
	Point3& operator-=(const Vector3& v);
	Point3& operator*=(float scalar);
	Point3& operator/=(float scalar);

	Vector3 operator-(const Point3& right) const;
};

class Point4 {
public:
	float x;
	float y;
	float z;
	float w;

	Point4();
	Point4(float x, float y, float z, float w);
	Point4(const Point4& other);
	Point4(const Vector4& v);
	Point4(const Point2& p);
	Point4(const Point3& p);
	Point4(float v);

	void print() const;

	float distance(const Point4& p) const;

	bool isZero() const;
	bool operator==(const Point4& other) const;

	Point4 operator+(const Vector4& v) const;
	Point4 operator-(const Vector4& v) const;
	Point4 operator*(float scalar) const;
	Point4 operator/(float scalar) const;

	Point4& operator+=(const Vector4& v);
	Point4& operator-=(const Vector4& v);
	Point4& operator*=(float scalar);
	Point4& operator/=(float scalar);

	Vector4 operator-(const Point4& right) const;
};

