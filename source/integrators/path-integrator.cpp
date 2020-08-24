#include "integrators/path-integrator.h"
#include "tools/util.h"

#include <algorithm>

PathIntegrator::PathIntegrator() { }
PathIntegrator::PathIntegrator(const PathIntegrator& other) { }

std::unique_ptr<Integrator> PathIntegrator::clone() const {
	return std::make_unique<PathIntegrator>();
}

Spectrum PathIntegrator::traceRay(const Ray& ray, const Scene& scene, Camera* camera, Sampler* sampler) {
	Spectrum L;
	Spectrum throughput(1.0f);
	Intersection intersection = scene.intersects(ray);
	Vector3 rayDirection = ray.direction;
	bool cameraRay = true;
	bool lastDistDelta = false;
	
	while (true) {
		if (!intersection.hit || intersection.light != nullptr) {
			if (cameraRay || lastDistDelta) {
				if (!intersection.hit) {
					L += throughput * scene.getSkybox()->emission(rayDirection);
				}

				else if (intersection.light != nullptr) {
					BSDF bsdf = intersection.material->createBSDF(intersection.intersectionPoint,
						intersection.wo);
					L += throughput * bsdf.evaluate(intersection.wo, intersection.wo);
				}
			}
			break;
		}


		intersection.intersectionPoint = intersection.material->bump(intersection.intersectionPoint);
		BSDF bsdf = intersection.material->createBSDF(intersection.intersectionPoint, intersection.wo);
		BSDFSample bsdfSample = bsdf.sample(sampler, intersection.wo);
		float cosTheta = glm::dot(intersection.intersectionPoint.shadingNormal, bsdfSample.sampledDirection);

		if (util::equals(bsdfSample.pdf, 0.0f) || bsdfSample.value.isZero() || util::equals(cosTheta, 0.0f)) {
			break;
		}

		if (!bsdfSample.isDeltaDist) {
			L += throughput * sampleDirectLighting(intersection.intersectionPoint, intersection.wo,
				bsdf, scene, sampler);
		}

		throughput *= (bsdfSample.value * std::abs(cosTheta)) / bsdfSample.pdf;
		lastDistDelta = bsdfSample.isDeltaDist;

		float rrProb = std::max(0.05f, 1.0f - throughput.luminance());
		if (sampler->getSample() < rrProb) {
			break;
		}
		throughput /= std::max((1.0f - rrProb), 0.1f);

		cameraRay = false;
		rayDirection = bsdfSample.sampledDirection;
		intersection = scene.intersects(intersection.intersectionPoint, bsdfSample.sampledDirection);
	}

	return L;
}

