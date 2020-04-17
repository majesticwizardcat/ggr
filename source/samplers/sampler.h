#pragma once

class Sampler;

#include "cameras/camera-sample.h"
#include "samplers/sample-2d.h"

#include <memory>

class Sampler {
protected:
	float m_resolutionWidth;
	float m_resolutionHeight;

public:
	Sampler();
	Sampler(const Sampler& other);
	Sampler(float resolutionWidth, float resolutionHeight);

	CameraSample getCameraSample(const Point2& rasterPosition);

	virtual float getSample() = 0;
	virtual Sample2D getSample2D() = 0;
	virtual std::unique_ptr<Sampler> clone() const = 0;
	virtual std::unique_ptr<Sampler> clone(int resW, int resH) const = 0;
};

