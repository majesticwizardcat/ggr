#include "bsdfs/bsdf.h"
#include "tools/shading-functions.h"

#include <utility>

BSDF::BSDF(const BSDF& other) : m_worldToShading(other.m_worldToShading),
	m_geometricNormal(other.m_geometricNormal) {
	for(int i = 0; i < other.m_bxdfs.size(); ++i) {
		m_bxdfs.push_back(std::move(other.m_bxdfs[i]->clone()));
	}
}

BSDF::BSDF(const SurfacePoint& point) : BSDF(point.shadingNormal, point.tangent,
	point.bitangent, point.geometricNormal) { }

BSDF::BSDF(const Vector3& shading, const Vector3& tangent,
	const Vector3& bitangent, const Vector3& geometric) :
	m_geometricNormal(geometric) {
	Matrix4 wts(tangent.x,   tangent.y,   tangent.z,   0.0f,
		    bitangent.x, bitangent.y, bitangent.z, 0.0f,
		    shading.x,   shading.y,   shading.z,   0.0f,
		    0.0f,        0.0f,        0.0f,        1.0f);

	m_worldToShading = Transformation(glm::transpose(wts), wts);
}

void BSDF::addBXDF(std::unique_ptr<BXDF>& bxdf) {
	m_bxdfs.push_back(std::move(bxdf));
}

Spectrum BSDF::evaluate(const Vector3& worldWo, const Vector3& worldWi) const {
	if (m_bxdfs.empty()) {
		return Spectrum();
	}
	Spectrum result;
	Vector3 wo = m_worldToShading.applyVector(worldWo);
	Vector3 wi = m_worldToShading.applyVector(worldWi);

	BXDFType type = glm::dot(m_geometricNormal, worldWo) * glm::dot(m_geometricNormal, worldWi) > 0
		? BXDFType::REFLECTION : BXDFType::REFRACTION;

	for (int i = 0; i < m_bxdfs.size(); ++i) {
		BXDFType t = m_bxdfs[i]->getType();
		if (t == type
			|| t == BXDFType::BOTH
			|| t == BXDFType::EMISSION) {
			result += m_bxdfs[i]->evaluate(wo, wi);
		}
	}

	return result;
}

float BSDF::pdf(const Vector3& worldWo, const Vector3& worldWi) const {
	if (m_bxdfs.empty()) {
		return 0.0f;
	}
	float result = 0.0f;
	Vector3 wo = m_worldToShading.applyVector(worldWo);
	Vector3 wi = m_worldToShading.applyVector(worldWi);

	BXDFType type = glm::dot(m_geometricNormal, worldWo) * glm::dot(m_geometricNormal, worldWi) > 0
		? BXDFType::REFLECTION : BXDFType::REFRACTION;

	int count = 0;
	for (int i = 0; i < m_bxdfs.size(); ++i) {
		BXDFType t = m_bxdfs[i]->getType();
		if (t == type
			|| t == BXDFType::BOTH
			|| t == BXDFType::EMISSION) {
			result += m_bxdfs[i]->pdf(wo, wi);
			count++;
		}
	}

	if (count > 1) {
		result /= (float) count;
	}

	return result;
}

BSDFSample BSDF::sample(Sampler* sampler, const Vector3& worldWo) const {
	if (m_bxdfs.empty()) {
		return BSDFSample();
	}
	Vector3 wo = m_worldToShading.applyVector(worldWo);
	int choice = sampler->getSample() * m_bxdfs.size();
	if (m_bxdfs[choice]->getType() == BXDFType::EMISSION) {
		return BSDFSample();
	}

	BSDFSample sample = m_bxdfs[choice]->sample(sampler, wo);

	if (sample.pdf == 0.0f) {
		return BSDFSample();
	}

	Vector3 wi = sample.sampledDirection;
	Vector3 worldWi = m_worldToShading.applyInverseVector(wi);
	sample.sampledDirection = worldWi;

	BXDFType type = glm::dot(m_geometricNormal, worldWo) * glm::dot(m_geometricNormal, worldWi) > 0
		? BXDFType::REFLECTION : BXDFType::REFRACTION;
	int count = 1;

	if (sample.type != type) {
		sample = BSDFSample(Spectrum(0.0f), 0.0f, worldWi, sample.isDeltaDist, type);
		count = 0;
	}

	if (sample.isDeltaDist) {
		return sample;
	}

	for (int i = 0; i < m_bxdfs.size(); ++i) {
		if (i != choice
			&& (m_bxdfs[i]->getType() == type || m_bxdfs[i]->getType() == BXDFType::BOTH)) {
			sample.value += m_bxdfs[i]->evaluate(wo, wi);
			sample.pdf += m_bxdfs[i]->pdf(wo, wi);
			count++;
		}
	}

	if (count > 1) {
		sample.pdf /= (float) count;
	}

	return sample;
}

