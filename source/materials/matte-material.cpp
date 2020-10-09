#include "materials/matte-material.h"
#include "textures/color-texture.h"
#include "bsdfs/diffuse-brdf.h"

#include <utility>

BSDF MatteMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point.shadingNormal, point.tangent, point.bitangent, point.geometricNormal);
	std::unique_ptr<BXDF> diffuse = std::make_unique<DiffuseBRDF>(m_diffuseColor->sample(point));
	bsdf.addBXDF(diffuse);
	return bsdf;
}

