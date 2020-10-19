#include "textures/height-map.h"

#include <algorithm>

HeightMap::HeightMap(const MipMap* mipmap) : ImageTexture(mipmap) { }

void HeightMap::bump(SurfacePoint* point) const {
	float du = std::max(0.0001f, 0.5f * (std::abs(point->dUVdx.x) + std::abs(point->dUVdy.x)));
	float dv = std::max(0.0001f, 0.5f * (std::abs(point->dUVdx.y) + std::abs(point->dUVdy.y)));
	Vector3 oldN = point->shadingNormal;

	SurfacePoint pdu(*point);
	SurfacePoint pdv(*point);
	pdu.uv.x += du;
	pdv.uv.y += dv;

	float heightU = sample(pdu).value();
	float heightV = sample(pdv).value();
	float height = sample(*point).value();

	point->dpdu += point->shadingNormal * ((heightU - height) / du);
	point->dpdv += point->shadingNormal * ((heightV - height) / dv);
	point->shadingNormal = glm::normalize(glm::cross(point->dpdu, point->dpdv));
	point->tangent = glm::normalize(point->dpdu);
	if (glm::dot(oldN, point->shadingNormal) < 0.0f) {
		point->shadingNormal *= -1.0f;
	}
	point->bitangent = glm::normalize(glm::cross(point->shadingNormal, point->tangent));
}

