#include "materials/transparent-material.h"
#include "bsdfs/specular-brdf.h"
#include "bsdfs/refraction-btdf.h"

TransparentMaterial::TransparentMaterial(const Texture* color, float materialIOR,
	float airIOR) : Material(), m_color(color), m_materialIOR(materialIOR), m_airIOR(airIOR) {
	m_fresnel = std::make_unique<FresnelCT>();
}

TransparentMaterial::TransparentMaterial(const Texture* color, float materialIOR)
	: TransparentMaterial(color, materialIOR, 1.0f) { }

BSDF TransparentMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	float IORin;
	float IORout;
	if (glm::dot(point.geometricNormal, wo) > 0) {
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

