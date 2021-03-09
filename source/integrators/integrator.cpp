#include "integrators/integrator.h"
#include "tools/util.h"

#include <algorithm>

Spectrum Integrator::sampleDirectLighting(const SurfacePoint& surfacePoint, const Vector3& wo,
	const Shader* surfaceShader, const Scene* scene, Sampler* sampler, Intersection* sampleIntersection,
	Spectrum* nextThroughput, bool* nextDistDelta) const {
	Spectrum L(0.0f);
	float bsdfSamplePDF;
	Vector3 sampledDirection;
	Spectrum bsdfValue = surfaceShader->sample(wo, &sampledDirection, &bsdfSamplePDF, nextDistDelta, sampler);
	scene->intersects(surfacePoint, sampledDirection, sampleIntersection);
	// Next throughput is bsdf * cosTheta / pdf, pdf is divided after sampling the bsdf
	// since the terms "bsdf * cosTheta" are being used in the sampling later
	*nextThroughput = bsdfValue * std::abs(glm::dot(surfacePoint.shadingNormal, sampledDirection));
	if (bsdfSamplePDF == 0.0f || bsdfValue.isZero()) {
		*nextThroughput = Spectrum(0.0f);
		return Spectrum(0.0f);
	}

	if (*nextDistDelta) {
		*nextThroughput /= bsdfSamplePDF;
		return L;
	}
	int lights = scene->getNumberOfLights();
	int choice = (int)std::floor((float)lights * sampler->getSample());
	const LightEntity* light = scene->getLight(choice);
	SurfacePoint sampledPoint;
	Vector3 lightDir;
	float lightPdf;
	float lightDist;
	Spectrum emission = light->sample(sampler, surfacePoint.point, &sampledPoint,
		&lightDir, &lightPdf, &lightDist);
	lightDir *= -1.0f;
	// Division with pdf is omitted since the balance heuristic weight
	// has the pdf on the numerator (both on light and bsdf MIS sampling)
	// MISWeight = pdf / (lpdf + bsdfPdf)
	// L = Le * bsdf * cosTheta * Weight / pdf
	// => L = Le * bsdf * cosTheta / (lpdf + bsdfPdf)
	if (lightPdf > 0.0f
		&& !emission.isZero()
		&& scene->areUnoccluded(surfacePoint, sampledPoint.point, lightDir, lightDist)) {
		L += (surfaceShader->evaluate(wo, lightDir)
			* emission)
			* (std::abs(glm::dot(surfacePoint.shadingNormal, lightDir))
			* (float)lights
				/ (lightPdf + surfaceShader->pdf(wo, lightDir)));
	}
	if (sampleIntersection->light && sampleIntersection->hit) {
		L += (*nextThroughput
			* sampleIntersection->light->emission(surfacePoint.point, sampleIntersection->intersectionPoint))
			/ (bsdfSamplePDF + sampleIntersection->light->pdf(surfacePoint.point, sampleIntersection->intersectionPoint));
	}
	*nextThroughput /= bsdfSamplePDF;
	return L;
}
