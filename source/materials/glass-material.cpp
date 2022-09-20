#include "materials/glass-material.h"
#include "tools/util.h"
#include "shaders/refraction-shader.h"
#include "shaders/mf-refraction-shader.h"

GlassMaterial::GlassMaterial(
		const Texture* color, const Texture* roughness, 
		float materialIOR, float airIOR) :
	Material(), m_color(color), 
	m_roughness(roughness), m_materialIOR(materialIOR), 
	m_airIOR(airIOR) { }

GlassMaterial::GlassMaterial(
		const Texture* color, const Texture* roughness,float materialIOR) :
	GlassMaterial(color, roughness, materialIOR, 1.0f) { }

std::unique_ptr<Shader> GlassMaterial::createShader(
		const SurfacePoint& point, const Vector3& wo) const {
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
	Spectrum color = m_color->sample(point);
	float roughness = util::clamp(m_roughness->sample(point).value(), 0.0f, 1.0f);
	roughness *= roughness;
	if (util::equals(roughness, 0.0f)) {
		return std::make_unique<RefractionShader>(
				point.shadingNormal, point.tangent, 
				point.bitangent, color, IORin, IORout);
	}
	return std::make_unique<MFRefractionShader>(
			point.shadingNormal, point.tangent, 
			point.bitangent, color, roughness, IORin, IORout);
}

