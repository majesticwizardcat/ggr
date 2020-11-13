#pragma once

class Sampler;

#include "cameras/camera-sample.h"
#include "samplers/sample-2d.h"

#include <queue>
#include <memory>

class Sampler {
private:
	CameraSample createCameraSample(const Point2& rasterPosition);

protected:
	float m_resolutionWidth;
	float m_resolutionHeight;
	std::queue<CameraSample> m_samples;

public:
	Sampler() = delete;
	Sampler(const Sampler& other) = delete;
	Sampler(float resolutionWidth, float resolutionHeight);

	CameraSample getCameraSample(const Point2& rasterPosition);
	virtual void createCameraSamples(const Point2& rasterPosition, int samples);

	virtual float getSample() = 0;
	virtual Sample2D getSample2D() = 0;
	virtual std::unique_ptr<Sampler> clone() const = 0;
	virtual std::unique_ptr<Sampler> clone(int resW, int resH) const = 0;
};

