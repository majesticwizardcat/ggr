#include "meshes/triangle.h"
#include "primitives/transformation.h"

#include <cmath>
#include <limits>

Triangle::Triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2) : v0(v0), v1(v1), v2(v2) { }

void Triangle::calculateGeometry() {
	m_degenerate = false;
	float u1u0 = v1->uv.x - v0->uv.x;
	float u2u0 = v2->uv.x - v0->uv.x;
	float v1v0 = v1->uv.y - v0->uv.y;
	float v2v0 = v2->uv.y - v0->uv.y;
	float dt = v2v0 * u1u0 - v1v0 * u2u0;

	if (dt == 0.0f) {
		// Arbitrarily assign dP/dUV partial derivatives
		Vector3 p1p0 = v1->position - v0->position;
		Vector3 p2p0 = v2->position - v0->position;
		m_dpdu = p1p0 - p2p0;
		m_dpdv = p2p0 - p1p0;
		m_area = glm::length(glm::cross(p1p0, p2p0)) / 2.0f;
		if (m_area == 0.0f) {
			m_degenerate = true;
			return;
		}
		m_geometricNormal = glm::normalize(glm::cross(m_dpdu, m_dpdv));
		m_tangent = glm::normalize(m_dpdu);
		m_area = glm::length(glm::cross(p1p0, p2p0)) / 2.0f;
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

bool Triangle::intersects(const Ray& ray, EntityIntersection* result) const {
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

	if (result->t < t || t < 0.0f) {
		return false;
	}

	result->t = t;
	result->triangleWeights.w0 = w0;
	result->triangleWeights.w1 = w1;
	result->triangleWeights.w2 = w2;
	return true;
}

void Triangle::fillIntersection(float w0, float w1, float w2, Intersection* result) const {
	result->intersectionPoint.point = v0->position * w0 + v1->position * w1 + v2->position * w2;
	result->intersectionPoint.uv = v0->uv * w0 + v1->uv * w1 + v2->uv * w2;
	result->intersectionPoint.shadingNormal = glm::normalize(v0->normal * w0 + v1->normal * w1 + v2->normal * w2);
	result->intersectionPoint.geometricNormal =
		glm::dot(m_geometricNormal, result->intersectionPoint.shadingNormal) > 0.0f ? m_geometricNormal : -m_geometricNormal;
	result->intersectionPoint.dpdu = m_dpdu;
	result->intersectionPoint.dpdv = m_dpdv;
	result->intersectionPoint.tangent = m_tangent;
	result->intersectionPoint.bitangent = glm::cross(result->intersectionPoint.shadingNormal, m_tangent);
	result->intersectionPoint.surfaceArea = m_area;
}

void Triangle::samplePoint(Sampler* sampler, SurfacePoint* sampledPoint) const {
	float u0 = std::sqrt(sampler->getSample());
	float w0 = 1.0f - u0;
	float w1 = sampler->getSample() * u0;
	float w2 = 1.0f - w0 - w1;

	sampledPoint->point = v0->position * w0 + v1->position * w1 + v2->position * w2;
	sampledPoint->uv = v0->uv * w0 + v1->uv * w1 + v2->uv * w2;
	sampledPoint->shadingNormal = m_geometricNormal;
	sampledPoint->tangent = m_tangent;
	sampledPoint->bitangent = glm::cross(sampledPoint->shadingNormal, m_tangent);
	sampledPoint->surfaceArea = m_area;
	sampledPoint->dUVdx = Vector2(0.0f);
	sampledPoint->dUVdy = Vector2(0.0f);
}
