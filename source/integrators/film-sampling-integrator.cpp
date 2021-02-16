#include "integrators/film-sampling-integrator.h"

#include <cmath>
#include <iostream>

void FilmSamplingIntegrator::setup(const Scene* scene, const Camera* camera, Film* film,
	Sampler* sampler, const RenderSettings& settings) {
	m_pixelsRendered = 0;
	m_initialSize = film->getWidth() * film->getHeight();
	int nextArray = 0;
	m_renderArrays
		= std::vector<std::vector<std::pair<unsigned int, unsigned int>>>(settings.threads);
	for (unsigned int x = 0; x < film->getWidth(); ++x) {
		for (unsigned int y = 0; y < film->getHeight(); ++y) {
			m_renderArrays[nextArray].push_back(std::make_pair(x, y));
			nextArray = (nextArray + 1) % settings.threads;
		}
	}
}

void FilmSamplingIntegrator::render(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const RenderSettings& settings) {
	const auto& pixelsToRender = m_renderArrays[m_nextWorkerIndex++];
	CameraSample cameraSample;
	Ray ray;
	Spectrum radiance;
	std::unique_ptr<Sampler> samplerClone = sampler->clone();
	for (const auto& nextPixel : pixelsToRender) {
		samplerClone->createCameraSamples(Point2((float)nextPixel.first,
			(float)nextPixel.second), settings.samples);
		for (unsigned int s = 0; s < settings.samples; ++s) {
			cameraSample = samplerClone->getCameraSample(
				Point2((float)nextPixel.first, (float)nextPixel.second));
			camera->generateRay(&ray, cameraSample);
			radiance = traceRay(&ray, scene, camera, samplerClone.get());
			film->addSample(nextPixel.first, nextPixel.second, cameraSample.filmPosition, radiance, ray.weight);
		}
	}
}

void FilmSamplingIntegrator::reset() {
	m_pixelsRendered = 0;
	m_initialSize = 0;
	m_renderArrays.clear();
	m_nextWorkerIndex = 0;
}

float FilmSamplingIntegrator::getCompletion() const {
	return (float) (m_initialSize - m_pixelsRendered) / (float) m_initialSize;
}
