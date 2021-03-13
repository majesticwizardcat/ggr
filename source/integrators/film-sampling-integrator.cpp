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
	int id = m_nextWorkerIndex++;
	const auto& pixelsToRender = m_renderArrays[id];
	std::unique_ptr<Sampler> samplerClone = sampler->clone();
	int done = 0;
	for (const auto& nextPixel : pixelsToRender) {
		samplerClone->createCameraSamples(nextPixel, settings.samples);
		renderPixel(scene, camera, film, samplerClone.get(), nextPixel, settings.samples);
		done++;
		if (id == 0 && done % 11 == 0) {
			std::cout << "\rCompeleted: " << (float)((float)done / (float)pixelsToRender.size()) * 100.0f
				<< " %                   ";
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
