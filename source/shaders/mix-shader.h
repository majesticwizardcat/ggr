#pragma once

class MixShader;

#include "shaders/shader.h"

class MixShader : public Shader {
private:
	std::unique_ptr<Shader> m_shader0;
	std::unique_ptr<Shader> m_shader1;
	float m_blend;
	float m_oneMinusBlend;

	Spectrum sampleMix(const Shader* sampling, const Shader* other, float blend0, float blend1,
		const Vector3& wo, Vector3* wi, float* pdf, bool* isDelta, Sampler* sampler) const {
		Spectrum value = sampling->sample(wo, wi, pdf, isDelta, sampler) * blend0;
		*pdf *= blend0;
		if (isDelta || other->isDelta()) {
			return value;
		}
		*pdf += blend1 * other->pdf(wo, *wi);
		value += other->evaluate(wo, *wi) * blend1;
		return value;
	}

public:
	MixShader() = delete;
	MixShader(const MixShader& other) = delete;
	MixShader(std::unique_ptr<Shader>& shader0,
		std::unique_ptr<Shader>& shader1, float blend) : 
		Shader(Vector3(0.0f), Vector3(0.0f), Vector3(0.0f)),
		m_shader0(std::move(shader0)), m_shader1(std::move(shader1)), 
		m_blend(1.0f - blend), m_oneMinusBlend(blend) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const {
		return m_shader0->evaluate(wo, wi) * m_blend + m_shader1->evaluate(wo, wi) * m_oneMinusBlend;
	}

	float pdf(const Vector3& wo, const Vector3& wi) const {
		return m_blend * m_shader0->pdf(wo, wi) + m_oneMinusBlend * m_shader1->pdf(wo, wi);
	}

	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const {
		float choice = sampler->getSample();
		if (choice < m_blend) {
			return sampleMix(m_shader0.get(), m_shader1.get(),
				m_blend, m_oneMinusBlend, wo, wi, pdf, isDelta, sampler);
		}
		return sampleMix(m_shader1.get(), m_shader0.get(),
			m_oneMinusBlend, m_blend, wo, wi, pdf, isDelta, sampler);
	}

	virtual bool isDelta() const {
		return m_shader0->isDelta() && m_shader1->isDelta();
	}
};
