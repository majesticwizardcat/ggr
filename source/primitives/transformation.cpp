#include "primitives/transformation.h"
#include "tools/util.h"

#include <cmath>
#include <glm/gtx/transform.hpp>

Transformation::Transformation() : Transformation(Matrix4(1.0f), Matrix4(1.0f)) { } 
Transformation::Transformation(const Transformation& other) : Transformation(other.m_transform, other.m_inverse) { }
Transformation::Transformation(const Matrix4& transform) : Transformation(transform, glm::inverse(transform)) { }
Transformation::Transformation(const Matrix4& transform, const Matrix4& inverse) : m_transform(transform), m_inverse(inverse) { }
Transformation::Transformation(float m00, float m10, float m20, float m30,
 			       float m01, float m11, float m21, float m31,
			       float m02, float m12, float m22, float m32,
		       	       float m03, float m13, float m23, float m33) :
		Transformation(Matrix4(m00, m10, m20, m30,
				       m01, m11, m21, m31,
				       m02, m12, m22, m32,
				       m03, m13, m23, m33)) { }

bool Transformation::operator==(const Transformation& other) const {
	return m_transform == other.m_transform;
}

Transformation Transformation::transpose() const {
	return Transformation(glm::transpose(m_transform), glm::transpose(m_inverse));
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
	return apply(Vector4(v, 0.0f));
}

Vector2 Transformation::apply(const Vector2& v) const {
	return apply(Vector4(v, 0.0f, 0.0f));
}

Normal Transformation::apply(const Normal& n) const {
	return glm::transpose(m_inverse) * Vector4(n, 0.0f);
}

Point4 Transformation::apply(const Point4& p) const {
	Point4 res = m_transform * p;

	if (!util::equals(res.w, 0.0f)) {
		res = res / res.w;
	}

	return res;
}

Point3 Transformation::apply(const Point3& p) const {
	return apply(Point4(p, 1.0f));
}

Point2 Transformation::apply(const Point2& p) const {
	return apply(Point4(p, 0.0f, 1.0f));
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
	return applyInverse(Vector4(v, 0.0f));
}

Vector2 Transformation::applyInverse(const Vector2& v) const {
	return applyInverse(Vector4(v, 0.0f, 0.0f));
}

Normal Transformation::applyInverse(const Normal& n) const {
	return glm::transpose(m_transform) * Vector4(n, 0.0f);
}

Point4 Transformation::applyInverse(const Point4& p) const {
	Point4 res = m_inverse * p;

	if (!util::equals(res.w, 0.0f)) {
		res = res / res.w;
	}

	return res;
}

Point3 Transformation::applyInverse(const Point3& p) const {
	return applyInverse(Point4(p, 1.0f));
}

Point2 Transformation::applyInverse(const Point2& p) const {
	return applyInverse(Point4(p, 0.0f, 1.0f));
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
	return Transformation(glm::translate(Vector3(x, y, z)));
}

Transformation transform::scale(float sx, float sy, float sz) {
	return Transformation(glm::scale(Vector3(sx, sy, sz)));
}

Transformation transform::rotateX(float radians) {
	return Transformation(glm::rotate(radians, Vector3(1.0f, 0.0f, 0.0f)));
}

Transformation transform::rotateY(float radians) {
	return Transformation(glm::rotate(radians, Vector3(0.0f, 1.0f, 0.0f)));
}

Transformation transform::rotateZ(float radians) {
	return Transformation(glm::rotate(radians, Vector3(0.0f, 0.0f, 1.0f)));
}

Transformation transform::rotate(float rx, float ry, float rz) {
	return rotateZ(rz) * rotateY(ry) * rotateX(rx);
}

Transformation transform::rotateAxis(float radians, const Vector3& axis) {
	return Transformation(glm::rotate(radians, axis));
}

Transformation transform::transform(float x, float y, float z,
			 float sx, float sy, float sz,
			 float rx, float ry, float rz) {

	return translate(x, y, z) * (rotate(rx, ry, rz) * scale(sx, sy, sz));
}

Transformation transform::lookAt(const Point3& position, const Point3& look, const Vector3& up) {
	return Transformation(glm::lookAt(position, look, up));
}

Transformation transform::view(const Point3& position, const Point3& look, const Vector3& up) {
	return lookAt(position, look, up).inverse();
}

Transformation transform::perspectiveProjection(float fov, float near, float far) {
	return Transformation(glm::perspective(fov, 1.0f, near, far));
}

Transformation transform::orthographicProjection(float near, float far) {
	return Transformation(glm::ortho(0.0f, 1.0f, 0.0f, 1.0f));
}

