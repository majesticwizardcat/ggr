#include "integrators/path-integrator.h"
#include "tools/util.h"

#include <algorithm>

Spectrum PathIntegrator::traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler) {
	Spectrum L;
	Spectrum throughput(1.0f);
	Spectrum nextThroughput;
	Intersection intersection;
	Vector3 rayDirection = ray->direction;
	BSDF bsdf;
	bool cameraRay = true;
	bool lastDistDelta = false;
	scene->intersects(ray, &intersection);

	while (true) {
		if (!intersection.hit) {
			L += throughput * scene->getSkybox()->emission(rayDirection);
			break;
		}

		if (intersection.light) {
			if (cameraRay || lastDistDelta) {
				bsdf = intersection.material->createBSDF(intersection.intersectionPoint,
					intersection.wo);
				L += throughput * bsdf.evaluate(intersection.wo, intersection.wo);
			}
			break;
		}

		intersection.material->bump(&intersection.intersectionPoint);
		bsdf = intersection.material->createBSDF(intersection.intersectionPoint, intersection.wo);

		L += throughput * sampleDirectLighting(
			SurfacePoint(intersection.intersectionPoint), Vector3(intersection.wo),
			bsdf, scene, sampler, &intersection, &nextThroughput, &lastDistDelta);

		throughput *= nextThroughput;
		float rrProb = std::max(0.05f, 1.0f - throughput.luminosity());
		if (sampler->getSample() < rrProb) {
			break;
		}
		throughput /= std::max((1.0f - rrProb), 0.1f);
		cameraRay = false;
	}

	return L;
}

