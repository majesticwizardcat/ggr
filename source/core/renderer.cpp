#include "core/renderer.h"
#include "tools/timing.h"

#include <iostream>
#include <thread>
#include <functional>

Renderer::Renderer(Scene* scene, Camera* camera, Integrator* integrator,
	Sampler* sampler, RenderSettings settings) : m_scene(scene), m_camera(camera),
	m_integrator(integrator), m_sampler(sampler), m_settings(settings) { }

Image Renderer::render() {
	Timer timer;
	timer.start();

	std::unique_ptr<Sampler> sampler = m_sampler->clone(m_settings.resolutionWidth, m_settings.resolutionHeight);
	m_scene->initializeAccelerator();
	m_integrator->reset();
	m_integrator->setup(m_scene, m_camera, sampler.get(), m_settings);

	auto worker = [&] () {
		while (!m_integrator->render(m_scene, m_camera, sampler.get(), m_settings)) {
			int percentage = (int) (m_integrator->getCompletion() * 100.0f);
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
	Image renderedFrame = m_integrator->combine();
	
	timer.stop();
	std::cout << "Finished rendering in: " << timer.getDuration().count() << " seconds" << std::endl;
	return renderedFrame;
}

