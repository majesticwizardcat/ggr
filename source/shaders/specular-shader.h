#pragma once

class SpecularShader;

#include "shaders/shader.h"
#include "shaders/fresnel.h"
#include "tools/shading-functions.h"
#include "tools/util.h"

class SpecularShader : public Shader {
private:
	Spectrum m_color;

public:
	SpecularShader() = delete;
	SpecularShader(const SpecularShader& other) = delete;
	SpecularShader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent, const Spectrum& color)
		: Shader(normal, tangent, bitangent), m_color(color) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const {
		return Spectrum(0.0f);
	}

	float pdf(const Vector3& wo, const Vector3& wi) const {
		return 0.0f;
	}

	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const {
		float cosTheta = glm::dot(wo, m_normal);
		*wi = shading::reflect(wo, m_normal, cosTheta);
		if (util::less(cosTheta, 0.0f)) {
			*pdf = 0.0f;
			return Spectrum(0.0f);
		}
		*pdf = 1.0f;
		*isDelta = true;
		return fresnel::fresnelSchlick(m_color, cosTheta) / cosTheta;
	}

	bool isDelta() const {
		return true;
	}
};
