#pragma once

class Light;

#include "lighting/light-sample.h"
#include "samplers/sampler.h"

class Light {
public:
	Light();
	Light(const Light& other);

	virtual Spectrum emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const = 0;
	virtual float pdf(const Point3& surfacePoint, const SurfacePoint& lightPoint) const = 0;
	virtual LightSample sample(Sampler* sampler, const Point3& surfacePoint) const = 0;
};

