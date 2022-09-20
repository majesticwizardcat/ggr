#pragma once

class MFRefractionShader;

#include "shaders/shader.h"

class MFRefractionShader : public Shader {
private:
	Spectrum m_color;
	float m_alpha;
	float m_eta;
	float m_IORin;
	float m_IORout;

public:
	MFRefractionShader() = delete;
	MFRefractionShader(const MFRefractionShader& other) = delete;
	MFRefractionShader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent, const Spectrum& color, float alpha,
		float IORin, float IORout) : 
		Shader(normal, tangent, bitangent),
		m_color(color), m_alpha(alpha), m_IORin(IORin), m_IORout(IORout),
		m_eta(IORout / IORin) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const;

	bool isDelta() const {
		return false;
	}
};
