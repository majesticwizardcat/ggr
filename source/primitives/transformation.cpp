#include "primitives/transformation.h"

#include <glm/gtx/transform.hpp>

Ray Transformation::applyRay(const Ray& ray) const {
	Ray r;
	r.origin = applyPoint(ray.origin);
	r.direction = applyVector(ray.direction);
	r.weight = ray.weight;
	r.isCameraRay = ray.isCameraRay;

	if (!r.isCameraRay) {
		return r;
	}

	r.dxOrigin = applyPoint(ray.dxOrigin);
	r.dxDirection = applyVector(ray.dxDirection);
	r.dyOrigin = applyPoint(ray.dyOrigin);
	r.dyDirection = applyVector(ray.dyDirection);
	return r;
}

Ray Transformation::applyInverseRay(const Ray& ray) const {
	Ray r;
	r.origin = applyInversePoint(ray.origin);
	r.direction = applyInverseVector(ray.direction);
	r.weight = ray.weight;
	r.isCameraRay = ray.isCameraRay;

	if (!r.isCameraRay) {
		return r;
	}

	r.dxOrigin = applyInversePoint(ray.dxOrigin);
	r.dxDirection = applyInverseVector(ray.dxDirection);
	r.dyOrigin = applyInversePoint(ray.dyOrigin);
	r.dyDirection = applyInverseVector(ray.dyDirection);
	return r;
}

Vertex Transformation::applyInverseVertex(const Vertex& v) const {
	Vertex result;
	result.position = applyInversePoint(v.position);
	result.normal = applyInverseVector(v.normal);
	result.uv = v.uv;
	return result;
}

Vertex Transformation::applyVertex(const Vertex& v) const {
Vertex result;
result.position = applyPoint(v.position);
result.normal = applyNormal(v.normal);
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

