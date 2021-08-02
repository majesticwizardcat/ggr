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
		const Vector3& bitangent, const Spectrum& color, float IORin, float IORout)
		: Shader(normal, tangent, bitangent), m_color(color), m_IORin(IORin),
		m_IORout(IORout), m_eta(IORout / IORin) { }
	RefractionShader(RefractionShader&& other) :
		Shader(std::move(other)),
		m_color(std::move(other.m_color)),
		m_IORin(std::move(other.m_IORin)),
		m_IORout(std::move(other.m_IORout)),
		m_eta(std::move(other.m_eta)) { }

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
