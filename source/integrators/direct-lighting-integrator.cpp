#include "integrators/direct-lighting-integrator.h"

Spectrum DirectLightingIntegrator::traceRay(const Ray& ray, const Scene* scene, const Camera* camera, Sampler* sampler) {
	Intersection intersection = scene->intersects(ray);
	Spectrum L;

	if (!intersection.hit) {
		L = scene->getSkybox()->emission(ray.direction);
	}

	else if (intersection.light != nullptr) {
		BSDF bsdf = intersection.material->createBSDF(intersection.intersectionPoint, intersection.wo);
		L = bsdf.evaluate(intersection.wo, intersection.wo);
	}

	else {
		intersection.intersectionPoint = intersection.material->bump(intersection.intersectionPoint);
		BSDF bsdf = intersection.material->createBSDF(intersection.intersectionPoint, intersection.wo);
		L = sampleDirectLighting(intersection.intersectionPoint, intersection.wo, bsdf, scene, sampler);
	}
	return L;
}

