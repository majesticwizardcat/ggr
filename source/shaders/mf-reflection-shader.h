#pragma once

class MFReflectionShader;

#include "shaders/shader.h"

class MFReflectionShader : public Shader {
private:
	Spectrum m_color;
	float m_alpha;

public:
	MFReflectionShader() = delete;
	MFReflectionShader(const MFReflectionShader& other) = delete;
	MFReflectionShader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent, const Spectrum& color, float alpha)
		: Shader(normal, tangent, bitangent), m_color(color), m_alpha(alpha) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const;

	bool isDelta() const {
		return false;
	}
};
