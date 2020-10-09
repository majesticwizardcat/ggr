#pragma once

class Integrator;

#include "core/scene.h"
#include "core/render-settings.h"
#include "cameras/camera.h"
#include "images/image.h"
#include "intersection/surface-point.h"
#include "bsdfs/bsdf.h"
#include "film/film.h"

#include <memory>

class Integrator {
protected:
	std::unique_ptr<Film> m_frame;

public:
	Integrator() { }
	Integrator(const Integrator& other) = delete;

	Spectrum sampleDirectLighting(const SurfacePoint& point, const Vector3& wo, const BSDF& surfaceBSDF,
		const Scene* scene, Sampler* sampler) const;

	virtual void setup(const Scene* scene, const Camera* camera, Sampler* sampler, const RenderSettings& settings) = 0;
	virtual bool render(const Scene* scene, const Camera* camera, Sampler* sampler, const RenderSettings& settings) = 0 ;
	virtual Image combine() = 0;
	virtual float getCompletion() const = 0;
	virtual void reset() = 0;
};

