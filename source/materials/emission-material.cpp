#include "materials/emission-material.h"
#include "bsdfs/emission-bxdf.h"

BSDF EmissionMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point);
	std::unique_ptr<BXDF> emission = std::make_unique<EmissionBXDF>(m_emission->sample(point) * m_intensity);
	bsdf.addBXDF(emission);
	return bsdf;
}

