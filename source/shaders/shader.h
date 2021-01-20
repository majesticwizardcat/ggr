#pragma once

class Shader;

#include "primitives/vector.h"
#include "spectra/spectrum.h"
#include "samplers/sampler.h"

class Shader {
protected:
	Vector3 m_normal;
	Vector3 m_tangent;
	Vector3 m_bitangent;

	inline void toWorld(Vector3* w) const {
		*w = w->x * m_tangent + w->y * m_bitangent + w->z * m_normal;
	}

public:
	Shader() = delete;
	Shader(const Shader& other) = delete;
	Shader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent) : m_normal(normal), m_tangent(tangent),
		m_bitangent(bitangent) { }
	virtual ~Shader() { }

	virtual Spectrum evaluate(const Vector3& wo, const Vector3& wi) const = 0;
	virtual float pdf(const Vector3& wo, const Vector3& wi) const = 0;
	virtual Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const = 0;
	virtual bool isDelta() const = 0;
};
