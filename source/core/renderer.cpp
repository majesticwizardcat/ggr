#include "core/renderer.h"
#include "tools/timing.h"

#include <iostream>
#include <thread>
#include <functional>

// Generally code is read vertically faster than horizontally
Renderer::Renderer(Scene* scene, Camera* camera, Integrator* integrator,
	Sampler* sampler, RenderSettings settings) : 
	m_scene(scene), m_camera(camera), m_integrator(integrator), 
	m_sampler(sampler), m_settings(settings) { }

Image Renderer::render() {
	Timer timer;
	auto film = std::make_unique<Film>(
			m_settings.resolutionWidth, 
			m_settings.resolutionHeight, 
			m_settings.filter);

	auto sampler = m_sampler->clone();
	m_scene->initializeAccelerator();
	m_integrator->setup(m_scene, m_camera, film.get(), sampler.get(), m_settings);

	auto worker = [&] () {
		m_integrator->render(
				m_scene, m_camera, film.get(), 
				sampler.get(), m_settings);
	};

	std::cout << "Starting rendering at: " << m_settings.resolutionWidth
		<< " x " << m_settings.resolutionHeight  
		<< " resolution, with: " << m_settings.samples
		<< " samples, using: " << m_settings.threads << " threads" << '\n';

	std::vector<std::thread> threads;
	timer.start();
	for (int i = 0; i < m_settings.threads; ++i) {
		threads.push_back(std::thread(worker));
	}

	for (std::thread& thread : threads) {
		thread.join();
	}

	std::cout << "\rCompleted: 100 %                           " << '\n';
	timer.stop();
	std::cout 
		<< "Finished rendering in: " 
		<< timer.getDuration().count() 
		<< " seconds" << '\n';
	return film->getImage();
}

