#include "meshes/triangle.h"
#include "primitives/transformation.h"

#include <cmath>
#include <limits>
#include <iostream>

Triangle::Triangle() : m_degenerate(true) { }

Triangle::Triangle(const std::shared_ptr<Vertex>& v0, const std::shared_ptr<Vertex>& v1,
	const std::shared_ptr<Vertex>& v2) : v0(v0), v1(v1), v2(v2), m_degenerate(false) {
	calculateGeometry();
}

Triangle::Triangle(const Triangle& other) : id(other.id), v0(other.v0), v1(other.v1), v2(other.v2),
	m_dpdu(other.m_dpdu), m_dpdv(m_dpdv), m_geometricNormal(other.m_geometricNormal),
	m_tangent(other.m_tangent), m_area(other.m_area), m_degenerate(other.m_degenerate) { }

void Triangle::calculateGeometry() {
	float u1u0 = v1->uv.x - v0->uv.x;
	float u2u0 = v2->uv.x - v0->uv.x;
	float v1v0 = v1->uv.y - v0->uv.y;
	float v2v0 = v2->uv.y - v0->uv.y;
	float dt = v2v0 * u1u0 - v1v0 * u2u0;

	if (dt == 0.0f) {
		m_degenerate = true;
		return;
	}

	float oneOverDt = 1.0f / dt;
	Vector3 p1p0 = v1->position - v0->position;
	Vector3 p2p0 = v2->position - v0->position;
	m_dpdu = (p1p0 * v2v0 - p2p0 * v1v0) * oneOverDt;
	m_dpdv = (p2p0 * u1u0 - p1p0 * u2u0) * oneOverDt;
	m_geometricNormal = m_dpdu.cross(m_dpdv).unit();
	m_tangent = m_dpdu.unit();

	m_area = p1p0.cross(p2p0).length() / 2.0f;
}

Vertex Triangle::interpolate(float w0, float w1, float w2) const {
	Vertex res;
	res.position = v0->position * w0 + v1->position * w1 + v2->position * w2;
	res.uv = v0->uv * w0 + v1->uv * w1 + v2->uv * w2;
	res.normal = (v0->normal * w0 + v1->normal * w1 + v2->normal * w2).unit();
	return res;
}

bool Triangle::intersects(const Ray& ray, Intersection* result) const {
	return intersects(ray, std::numeric_limits<float>::max(), result);
}

bool Triangle::intersects(const Ray& ray, float maxT, Intersection* result) const {
	Point3 p0 = ray.toRaySpace(v0->position);
	Point3 p1 = ray.toRaySpace(v1->position);
	Point3 p2 = ray.toRaySpace(v2->position);

	float y1y2 = p1.y - p2.y;
	float x2x1 = p2.x - p1.x;
	float y2y0 = p2.y - p0.y;
	float x0x2 = p0.x - p2.x;
	float det = (p1.y - p2.y) * x0x2 + x2x1 * (-y2y0);

	if (det == 0.0f) {
		return false;
	}
	
	float oneOverDet = 1.0f / det;

	float w0 = (-y1y2 * p2.x - x2x1 * p2.y) * oneOverDet;
	float w1 = (-y2y0 * p2.x - x0x2 * p2.y) * oneOverDet;
	float w2 = 1.0f - w0 - w1;

	if (w2 < 0.0f || w0 < 0.0f || w1 < 0.0f) {
		return false;
	}

	float t = p0.z * w0 + p1.z * w1 + p2.z * w2;

	if (maxT < t || t < 0.0f) {
		return false;
	}

	if (result == nullptr) {
		return true;
	}

	Vertex iv = interpolate(w0, w1, w2);
	Normal gn = m_geometricNormal;
	if (iv.normal.dot(m_geometricNormal) < 0.0f) {
		gn.flip();
	}

	result->t = t;
	result->intersectionPoint =
		SurfacePoint(iv, gn, m_dpdu, m_dpdv, m_tangent, iv.normal.cross(m_tangent), m_area, id);
	return true;
}

bool Triangle::intersects(const Ray& ray, float maxT) const {
	return intersects(ray, maxT, nullptr);
}

SurfacePoint Triangle::samplePoint(Sampler* sampler) const {
	float u0 = std::sqrt(sampler->getSample());

	float w0 = 1.0f - u0;
	float w1 = sampler->getSample() * u0;
	float w2 = 1.0f - w0 - w1;

	Vertex v = interpolate(w0, w1, w2);
	Normal gn = m_geometricNormal;
	if (v.normal.dot(m_geometricNormal) < 0) {
		gn.flip();
	}

	return SurfacePoint(v, gn, m_dpdu, m_dpdv, m_tangent, v.normal.cross(m_tangent), m_area, id);
}

float Triangle::getArea() const {
	return m_area;
}

