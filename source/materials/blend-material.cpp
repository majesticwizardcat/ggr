#include "materials/blend-material.h"
#include "tools/util.h"
#include "shaders/diffuse-shader.h"
#include "shaders/mf-reflection-shader.h"
#include "shaders/specular-shader.h"
#include "shaders/mix-shader.h"

Shader* BlendMaterial::createShader(const SurfacePoint& point, const Vector3& wo, StackAllocator& alloc) const {
	Spectrum diffuse = m_diffuse->sample(point);
	Spectrum glossy = m_glossy->sample(point);
	float roughness = m_roughness->sample(point).value();
	roughness *= roughness;

	Shader* difShader = alloc.construct<DiffuseShader>(DiffuseShader(point.shadingNormal, point.tangent, point.bitangent,
		diffuse));
	Shader* glossShader;
	if (util::equals(roughness, 0.0f)) {
		glossShader = alloc.construct<SpecularShader>(SpecularShader(point.shadingNormal, point.tangent, point.bitangent,
			glossy));
	}
	else {
		glossShader = alloc.construct<MFReflectionShader>(MFReflectionShader(point.shadingNormal, point.tangent,
			point.bitangent, glossy, roughness));
	}
	return alloc.construct<MixShader>(MixShader(difShader, glossShader, m_blend));
}
