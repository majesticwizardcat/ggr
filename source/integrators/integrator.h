#pragma once

class Integrator;

#include "core/scene.h"
#include "core/render-settings.h"
#include "cameras/camera.h"
#include "images/image.h"
#include "intersection/surface-point.h"
#include "bsdfs/bsdf.h"

#include <memory>

class Integrator {
public:
	Integrator();
	Integrator(const Integrator& other);

	Spectrum sampleDirectLighting(const SurfacePoint& point, const Vector3& wo, const BSDF& surfaceBSDF,
		const Scene& scene, Sampler* sampler) const;

	virtual void setup(const Scene& scene, Camera* camera, Sampler* sampler, const RenderSettings& settings) = 0;
	virtual bool render(const Scene& scene, Camera* camera, Sampler* sampler, const RenderSettings& settings) = 0;
	virtual Image combine() = 0;
	virtual float getCompletion() const = 0;
	virtual std::unique_ptr<Integrator> clone() const = 0;
};

