#pragma once

class Transformation;

#include "primitives/matrix.h"
#include "primitives/vector.h"
#include "primitives/point.h"
#include "intersection/ray.h"
#include "meshes/vertex.h"

class Transformation {
private:
	Matrix4 m_transform;
	Matrix4 m_inverse;

public:
	Transformation();
	Transformation(const Transformation& other);
	Transformation(const Matrix4& transform);
	Transformation(const Matrix4& transform, const Matrix4& inverse);
	Transformation(float m00, float m10, float m20, float m30,
		       float m01, float m11, float m21, float m31,
		       float m02, float m12, float m22, float m32,
		       float m03, float m13, float m23, float m33);

	void print() const;

	bool swapsHandedness() const;
	bool operator==(const Transformation& other) const;

	Transformation transpose() const;
	Transformation inverse() const;
	Transformation combine(const Transformation& right) const;
	Transformation operator*(const Transformation& right) const;

	Vector4 apply(const Vector4& v) const;
	Vector3 apply(const Vector3& v) const;
	Vector2 apply(const Vector2& v) const;
	Normal apply(const Normal& n) const;
	Point4 apply(const Point4& p) const;
	Point3 apply(const Point3& p) const;
	Point2 apply(const Point2& p) const;
	Ray apply(const Ray& r) const;
	Vertex apply(const Vertex& v) const;

	Vector4 applyInverse(const Vector4& v) const;
	Vector3 applyInverse(const Vector3& v) const;
	Vector2 applyInverse(const Vector2& v) const;
	Normal applyInverse(const Normal& n) const;
	Point4 applyInverse(const Point4& p) const;
	Point3 applyInverse(const Point3& p) const;
	Point2 applyInverse(const Point2& p) const;
	Ray applyInverse(const Ray& r) const;
	Vertex applyInverse(const Vertex& v) const;
};

namespace transform {
Transformation translate(float x, float y, float z);
Transformation scale(float sx, float sy, float sz);
Transformation rotateX(float radians);
Transformation rotateY(float radians);
Transformation rotateZ(float radians);
Transformation rotate(float rx, float ry, float rz);
Transformation rotateAxis(float radians, const Vector3& axis);
Transformation transform(float x, float y, float z,
			 float sx, float sy, float sz,
			 float rx, float ry, float rz);
Transformation lookAt(const Point3& position, const Point3& look, const Vector3& up);
Transformation view(const Point3& position, const Point3& look, const Vector3& up);
Transformation perspectiveProjection(float fov, float near, float far);
Transformation orthographicProjection(float near, float far);
}

