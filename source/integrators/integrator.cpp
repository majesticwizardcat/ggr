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
	if (*nextDistDelta) {
		if (util::equals(bsdfSamplePDF, 0.0f) || bsdfValue.isZero()) {
			*nextThroughput = Spectrum(0.0f);
			return L;
		}
		*nextThroughput /= bsdfSamplePDF;
		return L;
	}
	int lights = scene->getNumberOfLights();
	int choice = (int)std::floor((float)lights * sampler->getSample());
	const LightEntity* light = scene->getLight(choice);
	LightSample lightSample = light->sample(sampler, surfacePoint.point);
	// Division with pdf is omitted since the balance heuristic weight
	// has the pdf on the numerator (both on light and bsdf MIS sampling)
	// MISWeight = pdf / (lpdf + bsdfPdf)
	// L = Le * bsdf * cosTheta * Weight / pdf
	// => L = Le * bsdf * cosTheta / (lpdf + bsdfPdf)
	if (lightSample.pdf > 0.0f
		&& !lightSample.emission.isZero()
		&& scene->areUnoccluded(surfacePoint, lightSample.sampledPoint)) {
		Vector3 wl = glm::normalize(lightSample.sampledPoint.point - surfacePoint.point);
		float MISWeight = 1.0f / (lightSample.pdf + surfaceShader->pdf(wo, wl));
		L += surfaceShader->evaluate(wo, wl)
			* lightSample.emission
			* std::abs(glm::dot(surfacePoint.shadingNormal, wl))
			* MISWeight
			* (float)lights;
	}
	if (util::equals(bsdfSamplePDF, 0.0f) || bsdfValue.isZero()) {
		*nextThroughput = Spectrum(0.0);
		return L;
	}
	if (sampleIntersection->hit && sampleIntersection->light) {
		Spectrum lightEmission = sampleIntersection->light->
			emission(surfacePoint.point, sampleIntersection->intersectionPoint);
		float pdfLight = sampleIntersection->light->pdf(surfacePoint.point, sampleIntersection->intersectionPoint);
		float MISWeight = 1.0f / (bsdfSamplePDF + pdfLight);
		L += *nextThroughput
			* lightEmission
			* MISWeight;
	}
	*nextThroughput /= bsdfSamplePDF;
	return L;
}
