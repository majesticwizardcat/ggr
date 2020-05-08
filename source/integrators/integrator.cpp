#include "integrators/integrator.h"
#include "tools/util.h"

#include <algorithm>

Integrator::Integrator() { }
Integrator::Integrator(const Integrator& other) { }

Spectrum Integrator::sampleDirectLighting(const SurfacePoint& surfacePoint, const Vector3& wo,
	const BSDF& surfaceBSDF, const Scene& scene, Sampler* sampler) const {
	Spectrum L;
	const Skybox* skybox = scene.getSkybox();
	int lights = scene.getNumberOfLights() + 1;
	int choice = lights * sampler->getSample();
	if (choice == lights - 1) {
		SkyboxSample skySample = skybox->sample(sampler);
		skySample.pdf /= (float) lights;

		if (!util::equals(skySample.pdf, 0.0f)
			&& !skySample.emission.isZero()
			&& !scene.isIntersected(surfacePoint, skySample.sampledDirection)) {

			float MISWeight = skySample.pdf
				/ (skySample.pdf + surfaceBSDF.pdf(wo, skySample.sampledDirection));
			L += (surfaceBSDF.evaluate(wo, skySample.sampledDirection)
				* skySample.emission
				* std::abs(surfacePoint.shadingNormal.dot(skySample.sampledDirection))
				* MISWeight) / skySample.pdf;
		}
	}

	else {
		const LightEntity* light = scene.getLight(choice);
		LightSample lightSample = light->sample(sampler, surfacePoint.point);
		lightSample.pdf /= (float) lights;

		if (!util::equals(lightSample.pdf, 0.0f)
			&& !lightSample.emission.isZero()
			&& scene.areUnoccluded(surfacePoint, lightSample.sampledPoint)) {

			Vector3 wl = (lightSample.sampledPoint.point - surfacePoint.point).unit();
			float MISWeight = lightSample.pdf / (lightSample.pdf + surfaceBSDF.pdf(wo, wl));

			L += (surfaceBSDF.evaluate(wo, wl)
				* lightSample.emission
				* std::abs(surfacePoint.shadingNormal.dot(wl))
				* MISWeight) / lightSample.pdf;
		}
	}

	BSDFSample bsdfSample = surfaceBSDF.sample(sampler, wo);
	if (util::equals(bsdfSample.pdf, 0.0f) || bsdfSample.value.isZero()) {
		return L;
	}

	Intersection intersection = scene.intersects(surfacePoint, bsdfSample.sampledDirection);
	if (!intersection.hit) {
		float pdfSkybox = skybox->pdf(bsdfSample.sampledDirection) / lights;
		float MISWeight = bsdfSample.pdf / (bsdfSample.pdf + pdfSkybox);
		L += (bsdfSample.value
			* skybox->emission(bsdfSample.sampledDirection)
			* std::abs(surfacePoint.shadingNormal.dot(bsdfSample.sampledDirection))
			* MISWeight) / bsdfSample.pdf;
	}

	else if (intersection.light != nullptr) {
		Spectrum lightEmission = intersection.light->emission(surfacePoint.point, intersection.intersectionPoint);
		float pdfLight = intersection.light->pdf(surfacePoint.point, intersection.intersectionPoint) / lights;
		float MISWeight = bsdfSample.pdf / (bsdfSample.pdf + pdfLight);
		L += (bsdfSample.value
			* lightEmission
			* std::abs(surfacePoint.shadingNormal.dot(bsdfSample.sampledDirection))
			* MISWeight) / bsdfSample.pdf;
	}

	return L;
}

