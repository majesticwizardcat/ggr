#include "materials/blend-material.h"
#include "tools/util.h"
#include "shaders/diffuse-shader.h"
#include "shaders/mf-reflection-shader.h"
#include "shaders/specular-shader.h"
#include "shaders/mix-shader.h"

std::unique_ptr<Shader> BlendMaterial::createShader(const SurfacePoint& point, const Vector3& wo) const {
	Spectrum diffuse = m_diffuse->sample(point);
	Spectrum glossy = m_glossy->sample(point);
	float roughness = m_roughness->sample(point).value();
	roughness *= roughness;

	std::unique_ptr<Shader> difShader = std::make_unique<DiffuseShader>(point.shadingNormal, point.tangent,
		point.bitangent, diffuse);
	std::unique_ptr<Shader> glossShader;
	if (util::equals(roughness, 0.0f)) {
		glossShader = std::make_unique<SpecularShader>(point.shadingNormal, point.tangent,
			point.bitangent, glossy);
	}
	else {
		glossShader = std::make_unique<MFReflectionShader>(point.shadingNormal, point.tangent,
			point.bitangent, glossy, roughness);
	}
	return std::make_unique<MixShader>(difShader, glossShader, m_blend);
}
