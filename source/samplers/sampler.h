#pragma once

class Sampler;

#include "cameras/camera-sample.h"
#include "samplers/sample-2d.h"
#include "filters/filter.h"

#include <vector>
#include <memory>

class Sampler {
protected:
	std::vector<CameraSample> m_samples;
	const Filter* m_filmFilter;

public:
	Sampler() = delete;
	Sampler(const Sampler& other) = delete;
	Sampler(const Filter* filmFilter) : m_filmFilter(filmFilter) { }
	virtual ~Sampler() { }

	CameraSample getCameraSample(const Point2& rasterPosition);
	virtual void createCameraSamples(const Point2& rasterPosition, int samples);
	virtual float getSample() = 0;
	virtual Sample2D getSample2D() = 0;
	virtual std::unique_ptr<Sampler> clone() const = 0;
};
