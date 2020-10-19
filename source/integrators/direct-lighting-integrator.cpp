#include "integrators/direct-lighting-integrator.h"

Spectrum DirectLightingIntegrator::traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler) {
	Intersection intersection;
	scene->intersects(ray, &intersection);

	if (!intersection.hit) {
		return scene->getSkybox()->emission(ray->direction);
	}

	if (intersection.light != nullptr) {
		BSDF bsdf = intersection.material->createBSDF(intersection.intersectionPoint, intersection.wo);
		return bsdf.evaluate(intersection.wo, intersection.wo);
	}

	intersection.material->bump(&intersection.intersectionPoint);
	BSDF bsdf = intersection.material->createBSDF(intersection.intersectionPoint, intersection.wo);
	return sampleDirectLighting(intersection.intersectionPoint, intersection.wo, bsdf, scene, sampler);
}

