#include "materials/transparent-material.h"
#include "shaders/refraction-shader.h"

TransparentMaterial::TransparentMaterial(const Texture* color, float materialIOR,
	float airIOR) : Material(), m_color(color), m_materialIOR(materialIOR), m_airIOR(airIOR) {
}

TransparentMaterial::TransparentMaterial(const Texture* color, float materialIOR)
	: TransparentMaterial(color, materialIOR, 1.0f) { }

std::unique_ptr<Shader> TransparentMaterial::createShader(const SurfacePoint& point, const Vector3& wo) const {
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
	return std::make_unique<RefractionShader>(point.shadingNormal, point.tangent, point.bitangent,
		m_color->sample(point), IORin, IORout);
}
