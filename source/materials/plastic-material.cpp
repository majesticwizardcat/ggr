#include "materials/plastic-material.h"
#include "tools/util.h"
#include "shaders/specular-shader.h"
#include "shaders/mf-reflection-shader.h"
#include "shaders/diffuse-shader.h"
#include "shaders/mix-shader.h"

PlasticMaterial::PlasticMaterial(const Texture* color, const Texture* roughness) :
	PlasticMaterial(color, roughness, 0.5f) { }

PlasticMaterial::PlasticMaterial(const Texture* color, const Texture* roughness, float blend)
	: Material(), m_color(color), m_roughness(roughness), m_blend(blend) {
}

std::unique_ptr<Shader> PlasticMaterial::createShader(
		const SurfacePoint& point, const Vector3& wo) const {
	Spectrum color = m_color->sample(point);
	float roughness = util::clamp(m_roughness->sample(point).value(), 0.0f, 1.0f);
	roughness *= roughness;
	std::unique_ptr<Shader> diffuse =
		std::make_unique<DiffuseShader>(point.shadingNormal, point.tangent,
		point.bitangent, color);
	std::unique_ptr<Shader> gloss;
	if (util::equals(roughness, 0.0f)) {
		gloss = std::make_unique<SpecularShader>(
				point.shadingNormal, point.tangent, 
				point.bitangent, color);
		// return std::...
	}
	else {
		gloss = std::make_unique<MFReflectionShader>(
				point.shadingNormal, point.tangent, 
				point.bitangent, color, roughness);
	}
	return std::make_unique<MixShader>(diffuse, gloss, m_blend);
}
