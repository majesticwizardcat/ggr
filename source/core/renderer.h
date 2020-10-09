#pragma once

class Renderer;

#include "core/scene.h"
#include "core/render-settings.h"
#include "cameras/camera.h"
#include "integrators/integrator.h"
#include "images/image.h"
#include "samplers/sampler.h"

#include <memory>

class Renderer {
private:
	Scene* m_scene;
	Camera* m_camera;
	Integrator* m_integrator;
	Sampler* m_sampler;
	RenderSettings m_settings;

public:
	Renderer() = delete;
	Renderer(const Renderer& other) = delete;
	Renderer(Scene* scene, Camera* camera, Integrator* integrator,
		Sampler* sampler, RenderSettings settings);

	Image render();
};

