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
	Scene m_scene;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Integrator> m_integrator;
	std::unique_ptr<Sampler> m_sampler;
	RenderSettings m_settings;

public:
	Renderer();
	Renderer(const Renderer& other);
	Renderer(const Scene& scene, std::unique_ptr<Camera>& camera,
		std::unique_ptr<Integrator>& integrator, 
		std::unique_ptr<Sampler>& sampler, const RenderSettings& settings);

	Image render();
};

