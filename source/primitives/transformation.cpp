#include "primitives/transformation.h"
#include "tools/util.h"

#include <cmath>

Transformation::Transformation() : Transformation(Matrix4(1.0f), Matrix4(1.0f)) { } 
Transformation::Transformation(const Transformation& other) : Transformation(other.m_transform, other.m_inverse) { }
Transformation::Transformation(const Matrix4& transform) : Transformation(transform, transform.inverse()) { }
Transformation::Transformation(const Matrix4& transform, const Matrix4& inverse) : m_transform(transform), m_inverse(inverse) { }
Transformation::Transformation(float m00, float m10, float m20, float m30,
 			       float m01, float m11, float m21, float m31,
			       float m02, float m12, float m22, float m32,
		       	       float m03, float m13, float m23, float m33) :
		Transformation(Matrix4(m00, m10, m20, m30,
				       m01, m11, m21, m31,
				       m02, m12, m22, m32,
				       m03, m13, m23, m33)) { }

void Transformation::print() const {
	m_transform.print();
}

bool Transformation::swapsHandedness() const {
	return m_transform.upperLeft().det() < 0;
}

bool Transformation::operator==(const Transformation& other) const {
	return m_transform == other.m_transform;
}

Transformation Transformation::transpose() const {
	return Transformation(m_transform.transpose(), m_inverse.transpose());
}

Transformation Transformation::inverse() const {
	return Transformation(m_inverse, m_transform);
}

Transformation Transformation::combine(const Transformation& right) const {
	return Transformation(m_transform * right.m_transform, right.m_inverse * m_inverse);
}

Transformation Transformation::operator*(const Transformation& right) const {
	return combine(right);
}

Vector4 Transformation::apply(const Vector4& v) const {
	return m_transform * v;
}

Vector3 Transformation::apply(const Vector3& v) const {
	return apply(Vector4(v));
}

Vector2 Transformation::apply(const Vector2& v) const {
	return apply(Vector4(v));
}

Normal Transformation::apply(const Normal& n) const {
	return m_inverse.transpose() * Vector4(n);
}

Point4 Transformation::apply(const Point4& p) const {
	Point4 res = m_transform * p;

	if (!util::equals(res.w, 0.0f)) {
		res = res / res.w;
	}

	return res;
}

Point3 Transformation::apply(const Point3& p) const {
	return apply(Point4(p));
}

Point2 Transformation::apply(const Point2& p) const {
	return apply(Point4(p));
}

Ray Transformation::apply(const Ray& ray) const {
	Ray r;
	r.origin = apply(ray.origin);
	r.direction = apply(ray.direction);
	r.weight = ray.weight;
	r.isCameraRay = ray.isCameraRay;

	if (!r.isCameraRay) {
		return r;
	}

	r.dxOrigin = apply(ray.dxOrigin);
	r.dxDirection = apply(ray.dxDirection);
	r.dyOrigin = apply(ray.dyOrigin);
	r.dyDirection = apply(ray.dyDirection);
	return r;
}

Vertex Transformation::apply(const Vertex& v) const {
	Vertex result;
	result.position = apply(v.position);
	result.normal = apply(v.normal);
	result.uv = v.uv;
	return result;
}

Vector4 Transformation::applyInverse(const Vector4& v) const {
	return m_inverse * v;
}

Vector3 Transformation::applyInverse(const Vector3& v) const {
	return applyInverse(Vector4(v));
}

Vector2 Transformation::applyInverse(const Vector2& v) const {
	return applyInverse(Vector4(v));
}

Normal Transformation::applyInverse(const Normal& n) const {
	return m_transform.transpose() * Vector4(n);
}

Point4 Transformation::applyInverse(const Point4& p) const {
	Point4 res = m_inverse * p;

	if (!util::equals(res.w, 0.0f)) {
		res = res / res.w;
	}

	return res;
}

Point3 Transformation::applyInverse(const Point3& p) const {
	return applyInverse(Point4(p));
}

Point2 Transformation::applyInverse(const Point2& p) const {
	return applyInverse(Point4(p));
}

Ray Transformation::applyInverse(const Ray& ray) const {
	Ray r;
	r.origin = applyInverse(ray.origin);
	r.direction = applyInverse(ray.direction);
	r.weight = ray.weight;
	r.isCameraRay = ray.isCameraRay;

	if (!r.isCameraRay) {
		return r;
	}

	r.dxOrigin = applyInverse(ray.dxOrigin);
	r.dxDirection = applyInverse(ray.dxDirection);
	r.dyOrigin = applyInverse(ray.dyOrigin);
	r.dyDirection = applyInverse(ray.dyDirection);
	return r;
}

Vertex Transformation::applyInverse(const Vertex& v) const {
	Vertex result;
	result.position = applyInverse(v.position);
	result.normal = applyInverse(v.normal);
	result.uv = v.uv;
	return result;
}

Transformation transform::translate(float x, float y, float z) {
	Matrix4 transform(1.0f, 0.0f, 0.0f, x,
			  0.0f, 1.0f, 0.0f, y,
			  0.0f, 0.0f, 1.0f, z,
			  0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4 inverse(  1.0f, 0.0f, 0.0f, -x,
			  0.0f, 1.0f, 0.0f, -y,
			  0.0f, 0.0f, 1.0f, -z,
			  0.0f, 0.0f, 0.0f,  1.0f);

	return Transformation(transform, inverse);
}

Transformation transform::scale(float sx, float sy, float sz) {
	Matrix4 transform(sx,   0.0f, 0.0f, 0.0f,
			  0.0f, sy,   0.0f, 0.0f,
			  0.0f, 0.0f, sz,   0.0f,
			  0.0f, 0.0f, 0.0f, 1.0f);
	
	Matrix4 inverse(1.0f / sx, 0.0f,      0.0f,      0.0f,
			0.0f,      1.0f / sy, 0.0f,      0.0f,
			0.0f,      0.0f,      1.0f / sz, 0.0f,
			0.0f,      0.0f,      0.0f,      1.0f);

	return Transformation(transform, inverse);
}

Transformation transform::rotateX(float radians) {
	float sinTheta = std::sin(radians);
	float cosTheta = std::cos(radians);

	Matrix4 transform(1.0f, 0.0f,      0.0f,     0.0f,
			  0.0f, cosTheta, -sinTheta, 0.0f,
			  0.0f, sinTheta,  cosTheta, 0.0f,
			  0.0f, 0.0f,      0.0f,     1.0f);
	
	return Transformation(transform, transform.transpose());
}

Transformation transform::rotateY(float radians) {
	float sinTheta = std::sin(radians);
	float cosTheta = std::cos(radians);

	Matrix4 transform( cosTheta, 0.0f, sinTheta, 0.0f,
			   0.0f,     1.0f, 0.0f,     0.0f,
			  -sinTheta, 0.0f, cosTheta, 0.0f,
			   0.0f,     0.0f, 0.0f,     1.0f);
	
	return Transformation(transform, transform.transpose());
}

Transformation transform::rotateZ(float radians) {
	float sinTheta = std::sin(radians);
	float cosTheta = std::cos(radians);

	Matrix4 transform(cosTheta, -sinTheta, 0.0f,     0.0f,
			  sinTheta,  cosTheta, 0.0f,     0.0f,
			  0.0f,      0.0f,     1.0f,     0.0f,
			  0.0f,      0.0f,     0.0f,     1.0f);
	
	return Transformation(transform, transform.transpose());
}

Transformation transform::rotate(float rx, float ry, float rz) {
	return rotateZ(rz) * rotateY(ry) * rotateX(rx);
}

Transformation transform::rotateAxis(float radians, const Vector3& axis) {
	Vector3 a = axis.unit();
	float sinTheta = std::sin(radians);
	float cosTheta = std::cos(radians);

	Matrix4 transform;
	transform.set(0, 0, a.x * a.x + (1.0f - a.x * a.x) * cosTheta);
	transform.set(0, 1, a.x * a.y * (1.0f - cosTheta) - a.z * sinTheta);
	transform.set(0, 2, a.x * a.z * (1.0f - cosTheta) + a.y * sinTheta);

	transform.set(1, 0, a.x * a.y * (1.0f - cosTheta) + a.z * sinTheta);
	transform.set(1, 1, a.y * a.y + (1.0f - a.y * a.y) * cosTheta);
	transform.set(1, 2, a.y * a.z * (1.0f - cosTheta) - a.x * sinTheta);

	transform.set(2, 0, a.x * a.z * (1.0f - cosTheta) - a.y * sinTheta);
	transform.set(2, 1, a.y * a.z * (1.0f - cosTheta) + a.x * sinTheta);
	transform.set(2, 2, a.z * a.z * (1.0f - a.z * a.z) * cosTheta);

	transform.set(3, 3, 1.0f);

	return Transformation(transform, transform.transpose());
}

Transformation transform::transform(float x, float y, float z,
			 float sx, float sy, float sz,
			 float rx, float ry, float rz) {

	return translate(x, y, z) * (rotate(rx, ry, rz) * scale(sx, sy, sz));
}

Transformation transform::lookAt(const Point3& position, const Point3& look, const Vector3& up) {
	Vector3 direction = (look - position).unit();
	Vector3 right = (direction.cross(up)).unit();

	Matrix4 transform(right.x, up.x, direction.x, position.x,
			  right.y, up.y, direction.y, position.y,
			  right.z, up.z, direction.z, position.z,
			  0.0f,   0.0f, 0.0f,        1.0f);

	return Transformation(transform);
}

Transformation transform::view(const Point3& position, const Point3& look, const Vector3& up) {
	return lookAt(position, look, up).inverse();
}

Transformation transform::perspectiveProjection(float fov, float near, float far) {
	float invTanFov = 1.0f / std::tan(fov / 2);
	float fplusn = far + near;
	float fsubn = far - near;
	float fmuln = far * near;

	Matrix4 transform(invTanFov, 0.0f,      0.0f,         0.0f,
			  0.0f,      invTanFov, 0.0f,         0.0f,
			  0.0f,      0.0f,      far / fsubn, -fmuln / fsubn,
			  0.0f,      0.0f,      1.0f,         0.0f);

	return Transformation(transform);
}

Transformation transform::orthographicProjection(float near, float far) {
	float invfmn = 1.0f / (far - near);

	Matrix4 transform(1.0f, 0.0f, 0.0f,    0.0f,
			  0.0f, 1.0f, 0.0f,    0.0f,
			  0.0f, 0.0f, invfmn, -near * invfmn,
			  0.0f, 0.0f, 0.0f,    1.0f);

	return Transformation(transform);
}

