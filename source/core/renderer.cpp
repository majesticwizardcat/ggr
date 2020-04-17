#include "core/renderer.h"
#include "tools/timing.h"

#include <iostream>
#include <utility>
#include <thread>
#include <functional>
#include <vector>
#include <mutex>

Renderer::Renderer() { }
Renderer::Renderer(const Renderer& other) : m_scene(other.m_scene), m_settings(other.m_settings),
	m_camera(std::move(other.m_camera->clone())), m_integrator(std::move(other.m_integrator->clone())),
	m_sampler(std::move(other.m_sampler->clone())) { }
Renderer::Renderer(const Scene& scene, std::unique_ptr<Camera>& camera,
	std::unique_ptr<Integrator>& integrator,  std::unique_ptr<Sampler>& sampler, const RenderSettings& settings) :
	m_scene(scene), m_camera(std::move(camera)), m_integrator(std::move(integrator)),
	m_sampler(std::move(sampler)), m_settings(settings) { }

Image Renderer::render() const {
	Timer timer;
	timer.start();

	std::unique_ptr<Camera> camera = m_camera->clone();
	std::unique_ptr<Integrator> integrator = m_integrator->clone();
	std::unique_ptr<Sampler> sampler = m_sampler->clone(m_settings.resolutionWidth, m_settings.resolutionHeight);

	integrator->setup(m_scene, camera.get(), sampler.get(), m_settings);

	auto worker = [&] () {
		while (!integrator->render(m_scene, camera.get(), sampler.get(), m_settings)) {
			int percentage = (int) (integrator->getCompletion() * 100.0f);
			std::cout << "\rCompleted: " << percentage << "%";
		}
	};

	std::cout << "Starting rendering with: " << m_settings.samples
		<< " samples, using: " << m_settings.threads << " threads" << std::endl;

	std::vector<std::thread> threads;
	for (int i = 0; i < m_settings.threads; ++i) {
		threads.push_back(std::thread(worker));
	}

	for (std::thread& thread : threads) {
		thread.join();
	}

	std::cout << "\rCompleted: 100%" << std::endl;
	std::cout << "Combining result" << std::endl;
	Image renderedFrame = integrator->combine();
	
	timer.stop();
	std::cout << "Finished rendering in: " << timer.getDuration().count() << " seconds" << std::endl;
	return renderedFrame;
}

