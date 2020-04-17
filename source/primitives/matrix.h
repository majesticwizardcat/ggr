#pragma once

class Matrix2;
class Matrix3;
class Matrix4;

#include "primitives/vector.h"
#include "primitives/point.h"

class Matrix2 {
private:
	float m_data[2 * 2];

public:
	Matrix2();
	Matrix2(const Matrix2& other);
	Matrix2(const float data[]);
	Matrix2(float diagonal);
	Matrix2(float m00, float m10,
			float m01, float m11);

	void set(int col, int row, float value);
	void print() const;

	float det() const;
	float trace() const;
	float get(int col, int row) const;

	bool isInvertible() const;
	bool operator==(const Matrix2& right) const;

	Matrix2 multiply(const Matrix2& right) const;
	Matrix2 inverse() const;
	Matrix2 transpose() const;

	Matrix2 operator+(const Matrix2& right) const;
	Matrix2 operator-(const Matrix2& right) const;
	Matrix2 operator*(const Matrix2& right) const;
	Matrix2 operator*(float scalar) const;

	Matrix2& operator+=(const Matrix2& right);
	Matrix2& operator-=(const Matrix2& right);
	Matrix2& operator*=(const Matrix2& right);
	Matrix2& operator*=(float scalar);

	Vector2 multiply(const Vector2& right) const;
	Vector2 operator*(const Vector2& right) const;

	Point2 multiply(const Point2& right) const;
	Point2 operator*(const Point2& right) const;
};

class Matrix3 {
private:
	float m_data[3 * 3];

public:
	Matrix3();
	Matrix3(const Matrix3& other);
	Matrix3(const float data[]);
	Matrix3(float diagonal);
	Matrix3(float m00, float m10, float m20,
			float m01, float m11, float m21,
			float m02, float m12, float m22);

	void print() const;
	void set(int col, int row, float value);

	float det() const;
	float trace() const;
	float get(int col, int row) const;

	bool isInvertible() const;
	bool operator==(const Matrix3& right) const;

	Matrix3 multiply(const Matrix3& right) const;
	Matrix3 inverse() const;
	Matrix3 transpose() const;

	Matrix3 operator+(const Matrix3& right) const;
	Matrix3 operator-(const Matrix3& right) const;
	Matrix3 operator*(const Matrix3& right) const;
	Matrix3 operator*(float scalar) const;

	Matrix3& operator+=(const Matrix3& right);
	Matrix3& operator-=(const Matrix3& right);
	Matrix3& operator*=(const Matrix3& right);
	Matrix3& operator*=(float scalar);

	Vector3 multiply(const Vector3& right) const;
	Vector3 operator*(const Vector3& right) const;

	Point3 multiply(const Point3& right) const;
	Point3 operator*(const Point3& right) const;
};

class Matrix4 {
private:
	float m_data[4 * 4];

public:
	Matrix4();
	Matrix4(const Matrix4& other);
	Matrix4(const float data[]);
	Matrix4(float diagonal);
	Matrix4(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32,
			float m03, float m13, float m23, float m33);

	void print() const;
	void set(int col, int row, float value);

	float det() const;
	float trace() const;
	float get(int col, int row) const;

	bool isInvertible() const;
	bool operator==(const Matrix4& right) const;

	Matrix3 upperLeft() const;

	Matrix4 multiply(const Matrix4& right) const;
	Matrix4 inverse() const;
	Matrix4 transpose() const;

	Matrix4 operator+(const Matrix4& right) const;
	Matrix4 operator-(const Matrix4& right) const;
	Matrix4 operator*(const Matrix4& right) const;
	Matrix4 operator*(float scalar) const;

	Matrix4& operator+=(const Matrix4& right);
	Matrix4& operator-=(const Matrix4& right);
	Matrix4& operator*=(const Matrix4& right);
	Matrix4& operator*=(float scalar);

	Vector4 multiply(const Vector4& right) const;
	Vector4 operator*(const Vector4& right) const;

	Point4 multiply(const Point4& right) const;
	Point4 operator*(const Point4& right) const;
};

