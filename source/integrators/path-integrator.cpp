#include "integrators/path-integrator.h"
#include "tools/util.h"
#include "shaders/shader.h"

#include <algorithm>

void PathIntegrator::renderPixel(const Scene* scene, const Camera* camera, Film* film,
	Sampler* sampler, const Point2& pixel, unsigned int samples) const {
	Ray ray;
	Intersection intersection;
	int depth;
	bool lastDistDelta;
	CameraSample cameraSample;
	std::unique_ptr<Shader> surfaceShader;
	Spectrum L;
	Spectrum throughput;
	Spectrum nextThroughput;
	SurfacePoint prevIntersectionP;
	Vector3 prevWo;
	for (unsigned int s = 0; s < samples; ++s) {
		cameraSample = sampler->getCameraSample(pixel);
		camera->generateRay(&ray, cameraSample);
		L = Spectrum(0.0f);
		throughput = Spectrum(1.0f);
		depth = 0;
		lastDistDelta = false;
		scene->intersects(&ray, &intersection);
		while (true) {
			if (!intersection.hit) {
				L += throughput * scene->getSkybox()->emission(-intersection.wo);
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
			prevIntersectionP = intersection.intersectionPoint;
			prevWo = intersection.wo;

			L += throughput * sampleDirectLighting(prevIntersectionP, prevWo,
				surfaceShader.get(), scene, sampler, &intersection, &nextThroughput, &lastDistDelta);
			if (nextThroughput.isZero()) {
				break;
			}
			nextThroughput.clamp(0.0f, 10.0f);
			throughput *= nextThroughput;
			if (depth > 4) {
				float rrProb = std::max(0.05f, 1.0f - throughput.luminosity());
				if (sampler->getSample() < rrProb) {
					if (!intersection.hit) {
						L += throughput * scene->getSkybox()->emission(-intersection.wo);
					}
					break;
				}
				throughput /= std::max((1.0f - rrProb), 0.1f);
			}
			depth++;
		}
		film->addSample(pixel.x, pixel.y,
			cameraSample.filmPosition, L, ray.weight);
	}
}

