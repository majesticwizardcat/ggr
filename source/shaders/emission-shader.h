#pragma once

class EmissionShader;

#include "shaders/shader.h"

class EmissionShader : public Shader {
private:
	Spectrum m_emission;

public:
	EmissionShader() = delete;
	EmissionShader(const EmissionShader& other) = delete;
	EmissionShader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent, const Spectrum& emission)
		: Shader(normal, tangent, bitangent), m_emission(emission) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const {
		return m_emission;
	}

	float pdf(const Vector3& wo, const Vector3& wi) const {
		return 0.0f;
	}

	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}

	bool isDelta() const {
		return true;
	}
};
