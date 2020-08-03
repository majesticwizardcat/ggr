#include "textures/height-map.h"

#include <algorithm>

HeightMap::HeightMap(const HeightMap& other) : ImageTexture(other) { }
HeightMap::HeightMap(const std::shared_ptr<MipMap>& mipmap) : ImageTexture(mipmap) { }

SurfacePoint HeightMap::bump(const SurfacePoint& point) const {
	float du = std::max(0.0001f, 0.5f * (std::abs(point.dUVdx.x) + std::abs(point.dUVdy.x)));
	float dv = std::max(0.0001f, 0.5f * (std::abs(point.dUVdx.y) + std::abs(point.dUVdy.y)));

	SurfacePoint bumped(point);
	SurfacePoint pdu(point);
	SurfacePoint pdv(point);
	pdu.uv.x += du;
	pdv.uv.y += dv;

	float heightU = sample(pdu).value();
	float heightV = sample(pdv).value();
	float height = sample(point).value();

	bumped.dpdu = point.dpdu + (point.shadingNormal * ((heightU - height) / du));
	bumped.dpdv = point.dpdv + (point.shadingNormal * ((heightV - height) / dv));
	bumped.shadingNormal = bumped.dpdu.cross(bumped.dpdv).unit();
	bumped.tangent = bumped.dpdu.unit();
	if (bumped.shadingNormal.dot(point.shadingNormal) < 0.0f) {
		bumped.shadingNormal.flip();
	}
	bumped.bitangent = bumped.shadingNormal.cross(bumped.tangent).unit();
	return bumped;
}

