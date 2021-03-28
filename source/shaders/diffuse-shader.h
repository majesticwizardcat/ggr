#pragma once

class DiffuseShader;

#include "shaders/shader.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

class DiffuseShader : public Shader {
private:
	Spectrum m_diffuseColor;

public:
	DiffuseShader() = delete;
	DiffuseShader(const DiffuseShader& other) = delete;
	DiffuseShader(const Vector3& normal, const Vector3& tangent,
		const Vector3& bitangent, const Spectrum& color)
		: Shader(normal, tangent, bitangent), m_diffuseColor(color * INV_PI) { }

	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const {
		return m_diffuseColor;
	}

	float pdf(const Vector3& wo, const Vector3& wi) const {
		return std::abs(glm::dot(m_normal, wi)) * INV_PI;
	}

	Spectrum sample(const Vector3& wo, Vector3* wi, float* pdf,
		bool* isDelta, Sampler* sampler) const {
		float r = std::sqrt(sampler->getSample());
		float phi = TWO_PI * sampler->getSample();
		Point2 disk(r * std::cos(phi), r * std::sin(phi));
		*wi = Vector3(disk, std::sqrt(std::max(0.0f, 1.0f - glm::dot(disk, disk))));
		float cosThetaWi = wi->z;
		float cosThetaWo = glm::dot(m_normal, wo);
		if (util::equals(cosThetaWo * cosThetaWi, 0.0f)) {
			*pdf = 0.0f;
			return Spectrum(0.0f);
		}

		if (cosThetaWi * cosThetaWo < 0.0f) {
			wi->z *= -1.0f;
		}
		toWorld(wi);
		*pdf = std::abs(cosThetaWi) * INV_PI;
		*isDelta = false;
		return m_diffuseColor;
	}

	bool isDelta() const {
		return false;
	}
};
