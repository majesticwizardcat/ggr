#pragma once

class BSDF;

#include "bsdfs/bxdf.h"
#include "samplers/sampler.h"
#include "primitives/vector.h"
#include "primitives/transformation.h"
#include "intersection/surface-point.h"

#include <vector>
#include <memory>

class BSDF {
private:
	std::vector<std::shared_ptr<BXDF>> m_bxdfs;
	Transformation m_worldToShading;
	Vector3 m_geometricNormal;

public:
	BSDF() = delete;
	BSDF(const BSDF& other);
	BSDF(const SurfacePoint& other);
	BSDF(const Vector3& shading, const Vector3& tangent,
		const Vector3& bitangent, const Vector3& geometric);

	void addBXDF(std::unique_ptr<BXDF>& bxdf);

	Spectrum evaluate(const Vector3& worldWo, const Vector3& constWi) const;
	float pdf(const Vector3& worldWo, const Vector3& worldWi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& worldWo) const;
};

