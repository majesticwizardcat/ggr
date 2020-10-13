#include "integrators/path-integrator.h"
#include "tools/util.h"

#include <algorithm>

Spectrum PathIntegrator::traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler) {
	Spectrum L;
	Spectrum throughput(1.0f);
	Intersection intersection;
	Vector3 rayDirection = ray->direction;
	bool cameraRay = true;
	bool lastDistDelta = false;
	scene->intersects(ray, &intersection);

	while (true) {
		if (!intersection.hit || intersection.light) {
			if (cameraRay || lastDistDelta) {
				if (!intersection.hit) {
					L += throughput * scene->getSkybox()->emission(rayDirection);
				}

				else if (intersection.light) {
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
		scene->intersects(intersection.intersectionPoint, bsdfSample.sampledDirection, &intersection);
	}

	return L;
}

