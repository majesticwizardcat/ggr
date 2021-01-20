#include "integrators/path-integrator.h"
#include "tools/util.h"
#include "shaders/shader.h"

#include <algorithm>

Spectrum PathIntegrator::traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler) {
	Spectrum L;
	Spectrum throughput(1.0f);
	Spectrum nextThroughput;
	Intersection intersection;
	Vector3 rayDirection = ray->direction;
	std::unique_ptr<Shader> surfaceShader;
	int depth = 0;
	bool lastDistDelta = false;
	scene->intersects(ray, &intersection);

	while (true) {
		if (!intersection.hit) {
			L += throughput * scene->getSkybox()->emission(rayDirection);
			break;
		}

		if (intersection.light) {
			if (depth == 0 || lastDistDelta) {
				surfaceShader = intersection.material->createShader(intersection.intersectionPoint,
					intersection.wo);
				L += throughput * surfaceShader->evaluate(intersection.wo, intersection.wo);
			}
			break;
		}

		intersection.material->bump(&intersection.intersectionPoint);
		surfaceShader = intersection.material->createShader(intersection.intersectionPoint, intersection.wo);

		L += throughput * sampleDirectLighting(
			SurfacePoint(intersection.intersectionPoint), Vector3(intersection.wo),
			surfaceShader.get(), scene, sampler, &intersection, &nextThroughput, &lastDistDelta);

		throughput *= nextThroughput;
		if (depth > 4) {
			float rrProb = std::max(0.05f, 1.0f - throughput.luminosity());
			if (sampler->getSample() < rrProb) {
				break;
			}
			throughput /= std::max((1.0f - rrProb), 0.1f);
		}
		depth++;
	}
	return L;
}

