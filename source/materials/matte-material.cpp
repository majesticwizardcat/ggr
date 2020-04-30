#include "materials/matte-material.h"
#include "textures/color-texture.h"
#include "bsdfs/diffuse-brdf.h"

#include <utility>

MatteMaterial::MatteMaterial(const MatteMaterial& other) : MatteMaterial(other.m_diffuseColor) { }
MatteMaterial::MatteMaterial(const std::shared_ptr<Texture>& diffuseColor) : m_diffuseColor(diffuseColor) { }

BSDF MatteMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point.shadingNormal, point.tangent, point.bitangent, point.geometricNormal);
	std::unique_ptr<BXDF> diffuse = std::make_unique<DiffuseBRDF>(m_diffuseColor->sample(point));
	bsdf.addBXDF(diffuse);
	return bsdf;
}

