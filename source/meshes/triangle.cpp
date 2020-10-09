#include "meshes/triangle.h"
#include "primitives/transformation.h"

#include <cmath>
#include <limits>
#include <iostream>

Triangle::Triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2) : v0(v0), v1(v1), v2(v2) { }

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
	m_geometricNormal = glm::normalize(glm::cross(m_dpdu, m_dpdv));
	m_tangent = glm::normalize(m_dpdu);

	m_area = glm::length(glm::cross(p1p0, p2p0)) / 2.0f;
}

Vertex Triangle::interpolate(float w0, float w1, float w2) const {
	Vertex res;
	res.position = v0->position * w0 + v1->position * w1 + v2->position * w2;
	res.uv = v0->uv * w0 + v1->uv * w1 + v2->uv * w2;
	res.normal = glm::normalize(v0->normal * w0 + v1->normal * w1 + v2->normal * w2);
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
	Vector3 gn = m_geometricNormal;
	if (glm::dot(m_geometricNormal, iv.normal) < 0.0f) {
		gn *= -1.0f;
	}

	result->t = t;
	result->intersectionPoint.point = iv.position;
	result->intersectionPoint.uv = iv.uv;
	result->intersectionPoint.geometricNormal = gn;
	result->intersectionPoint.dpdu = m_dpdu;
	result->intersectionPoint.dpdv = m_dpdv;
	result->intersectionPoint.shadingNormal = iv.normal;
	result->intersectionPoint.tangent = m_tangent;
	result->intersectionPoint.bitangent = glm::cross(iv.normal, m_tangent);
	result->intersectionPoint.surfaceArea = m_area;
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
	Vector3 gn = m_geometricNormal;
	if (glm::dot(m_geometricNormal, v.normal) < 0.0f) {
		gn *= -1.0f;
	}

	SurfacePoint sampled;
	sampled.point = v.position;
	sampled.uv = v.uv;
	sampled.geometricNormal = gn;
	sampled.dpdu = m_dpdu;
	sampled.dpdv = m_dpdv;
	sampled.shadingNormal = v.normal;
	sampled.tangent = m_tangent;
	sampled.bitangent = glm::cross(v.normal, m_tangent);
	sampled.surfaceArea = m_area;
	return sampled;
}
