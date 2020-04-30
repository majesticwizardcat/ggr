#include "materials/transparent-material.h"
#include "bsdfs/specular-brdf.h"
#include "bsdfs/refraction-btdf.h"

TransparentMaterial::TransparentMaterial(const TransparentMaterial& other) : m_color(other.m_color),
	m_materialIOR(other.m_materialIOR), m_airIOR(other.m_airIOR) { }
TransparentMaterial::TransparentMaterial(const std::shared_ptr<Texture>& color, float materialIOR,
	float airIOR) : m_color(color), m_materialIOR(materialIOR), m_airIOR(airIOR) {
	m_fresnel = std::make_unique<FresnelCT>();
}
TransparentMaterial::TransparentMaterial(const std::shared_ptr<Texture>& color, float materialIOR)
	: TransparentMaterial(color, materialIOR, 1.0f) { }

BSDF TransparentMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	float IORin;
	float IORout;
	if (point.geometricNormal.dot(wo) > 0) {
		IORin = m_airIOR;
		IORout = m_materialIOR;
	}

	else {
		IORin = m_materialIOR;
		IORout = m_airIOR;
	}

	std::unique_ptr<BXDF> refr =
		std::make_unique<RefractionBTDF>(m_color->sample(point), m_fresnel.get(), IORin, IORout);
	BSDF bsdf(point);
	bsdf.addBXDF(refr);
	return bsdf;
}

