#include "integrators/film-sampling-integrator.h"

#include <cmath>
#include <iostream>

void FilmSamplingIntegrator::setup(const Scene* scene, const Camera* camera, Film* film,
	Sampler* sampler, const RenderSettings& settings) {
	m_pixelsRendered = 0;
	m_initialSize = film->getWidth() * film->getHeight();
	int nextArray = 0;
	m_renderArrays = std::vector<std::vector<Point2>>(settings.threads);
	for (unsigned int x = 0; x < film->getWidth(); ++x) {
		for (unsigned int y = 0; y < film->getHeight(); ++y) {
			m_renderArrays[nextArray].push_back(Point2((float) x, (float) y));
			nextArray = (nextArray + 1) % settings.threads;
		}
	}
}

void FilmSamplingIntegrator::render(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const RenderSettings& settings) {
	const auto& pixelsToRender = m_renderArrays[m_nextWorkerIndex++];
	std::unique_ptr<Sampler> samplerClone = sampler->clone();
	for (const auto& nextPixel : pixelsToRender) {
		samplerClone->createCameraSamples(nextPixel, settings.samples);
		renderPixel(scene, camera, film, samplerClone.get(), nextPixel, settings.samples);
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
