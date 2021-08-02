#include "integrators/path-integrator.h"
#include "tools/util.h"
#include "shaders/shader.h"
#include "tools/stack-allocator.h"

#include <algorithm>

void PathIntegrator::renderPixel(const Scene* scene, const Camera* camera, Film* film,
	Sampler* sampler, const Point2& pixel, unsigned int samples) const {
	byte buffer[1024];
	StackAllocator alloc(buffer, sizeof(buffer));
	Ray ray;
	Intersection intersection;
	float lumAccumulation;
	float lastLumL;
	float newAccum;
	float newLumL;
	bool lastDistDelta;
	CameraSample cameraSample;
	Shader* surfaceShader;
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
		lumAccumulation = 0.0f;
		lastLumL = 0.0f;
		newLumL = 0.0f;
		lastDistDelta = false;
		scene->intersects(&ray, &intersection);
		while (true) {
			alloc.clearMemory();
			if (!intersection.hit) {
				L += throughput * scene->getSkybox()->emission(-intersection.wo);
				break;
			}
			if (intersection.light) {
				if (lumAccumulation == 0.0f || lastDistDelta) {
					surfaceShader = intersection.material->createShader(intersection.intersectionPoint,
						intersection.wo, alloc);
					L += throughput * surfaceShader->evaluate(intersection.wo, intersection.wo);
				}
				break;
			}

			intersection.material->bump(&intersection.intersectionPoint);
			surfaceShader = intersection.material->createShader(intersection.intersectionPoint, intersection.wo, alloc);
			prevIntersectionP = intersection.intersectionPoint;
			prevWo = intersection.wo;

			L += throughput * sampleDirectLighting(prevIntersectionP, prevWo,
				surfaceShader, scene, sampler, &intersection, &nextThroughput, &lastDistDelta, alloc);
			if (nextThroughput.isZero()) {
				break;
			}
			throughput *= nextThroughput;
			if (lumAccumulation > 4.0f) {
				if (!intersection.hit) {
					L += throughput * scene->getSkybox()->emission(-intersection.wo);
				}
				break;
			}
			newLumL = util::clamp(L.luminosity(), 0.0f, 1.0f);
			lastLumL = newLumL;
			newAccum = (util::clamp(nextThroughput.luminosity(), 0.0f, 1.0f) + newLumL - lastLumL) * 0.5f;;
			lumAccumulation += 1.1f - newAccum;
		}
		film->addSample(pixel.x, pixel.y,
			cameraSample.filmPosition, L, ray.weight);
	}
}

