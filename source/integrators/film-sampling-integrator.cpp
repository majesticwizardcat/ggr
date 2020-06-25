#include "integrators/film-sampling-integrator.h"

#include <cmath>

FilmSamplingIntegrator::FilmSamplingIntegrator() : m_initialSize(0) { }
FilmSamplingIntegrator::FilmSamplingIntegrator(const Integrator& other) { }

void FilmSamplingIntegrator::setup(const Scene& scene, Camera* camera, Sampler* sampler, const RenderSettings& settings) {
	m_frame = std::make_unique<Film>(settings.resolutionWidth, settings.resolutionHeight, settings.filter);
	m_tiles = m_frame->createUnfilteredFilmTiles(settings.tileSize);
	m_initialSize = m_tiles.size();
}

bool FilmSamplingIntegrator::render(const Scene& scene, Camera* camera, Sampler* sampler, const RenderSettings& settings) {
	m_tilesLock.lock();
	if (m_tiles.empty()) {
		m_tilesLock.unlock();
		return true;
	}

	UnfilteredFilmTile tile = m_tiles.back();
	m_tiles.pop_back();
	m_tilesLock.unlock();

	float padX = std::ceil(2.0f + settings.filter->getWidth());
	float padY = std::ceil(2.0f + settings.filter->getHeight());
	Point2 freeBoxStart(tile.tileStartX + padX, tile.tileStartY + padY);
	Point2 freeBoxEnd(std::max(freeBoxStart.x, tile.tileEndX - padX),
			  std::max(freeBoxStart.y, tile.tileEndY - padY));

	std::unique_ptr<Sampler> samplerClone = sampler->clone();
	CameraSample cameraSample;
	Ray ray;
	for (int x = tile.tileStartX; x < tile.tileEndX; ++x) {
		for (int y = tile.tileStartY; y < tile.tileEndY; ++y) {
			samplerClone->createCameraSamples(Point2(x, y), settings.samples);
			for (int s = 0; s < settings.samples; ++s) {
				cameraSample = samplerClone->getCameraSample(Point2(x, y));
				ray = camera->generateRay(cameraSample);
				if (x > freeBoxStart.x && x < freeBoxEnd.x
					&& y > freeBoxStart.y && y < freeBoxEnd.y) {
					m_frame->addUnfilteredSample(
						UnfilteredFilmSample(cameraSample.filmPosition,
						traceRay(ray, scene, camera, samplerClone.get()),
						ray.weight));
				}
				else {
					tile.samples.push_back(
						UnfilteredFilmSample(cameraSample.filmPosition,
						traceRay(ray, scene, camera, samplerClone.get()),
						ray.weight));
				}
				if (tile.samples.size() > 0 && m_filmLock.try_lock()) {
					m_frame->mergeFilmTile(tile);
					m_filmLock.unlock();
					tile.samples.clear();
				}
			}
		}
	}

	bool finished = false;
	while (!m_filmLock.try_lock()) {
		finished = render(scene, camera, sampler, settings);
	}
	m_frame->mergeFilmTile(tile);
	m_filmLock.unlock();
	return finished;
}

float FilmSamplingIntegrator::getCompletion() const {
	return (float) (m_initialSize - m_tiles.size()) / (float) m_initialSize;
}

Image FilmSamplingIntegrator::combine() {
	return m_frame->getImage();
}

