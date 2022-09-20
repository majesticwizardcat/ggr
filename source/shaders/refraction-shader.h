#pragma once

class RefractionShader;

#include "shaders/shader.h"
#include "shaders/fresnel.h"

class RefractionShader : public Shader {
private:
	Spectrum m_color;
	float m_IORin;
	float m_IORout;
	float m_eta;

public:
	RefractionShader() = delete;
	RefractionShader(const RefractionShader& other) = delete;
	RefractionShader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent, const Spectrum& color, 
		float IORin, float IORout) : // This is a separation of logic, mark it
		Shader(normal, tangent, bitangent), 
		m_color(color), m_IORin(IORin),
		m_IORout(IORout), m_eta(IORout / IORin) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const {
		return Spectrum(0.0f);
	}

	float pdf(const Vector3& wo, const Vector3& wi) const {
		return 0.0f;
	}

	bool isDelta() const {
		return true;
	}

	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const;
};
