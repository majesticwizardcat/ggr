#include "integrators/path-integrator.h"
#include "entities/entity.h"
#include "entities/light-entity.h"
#include "meshes/cube.h"
#include "meshes/plane.h"
#include "meshes/sphere.h"
#include "primitives/transformation.h"
#include "tools/constants.h"
#include "materials/matte-material.h"
#include "materials/emission-material.h"
#include "materials/metal-material.h"
#include "materials/plastic-material.h"
#include "materials/glass-material.h"
#include "samplers/halton-sampler.h"
#include "cameras/perspective-camera.h"
#include "textures/color-texture.h"
#include "textures/image-texture.h"
#include "textures/height-map.h"
#include "film/film.h"
#include "images/image-io.h"
#include "images/mipmap.h"
#include "images/checkerboard-image.h"
#include "core/scene.h"
#include "core/renderer.h"
#include "core/render-settings.h"
#include "filters/gaussian-filter.h"
#include "samplers/stohastic-sampler.h"
#include "meshes/mesh-loader.h"

#include <iostream>
#include <memory>

const int DEFAULT_SAMPLES = 2123;
const int DEFAULT_THREADS = 4;

int main(int args, char** argv) {
	int samples = DEFAULT_SAMPLES; int threads = DEFAULT_THREADS;
	if (args > 2) {
		samples = atoi(argv[1]);
		threads = atoi(argv[2]);
	}

	auto eifel = meshloader::loadObj("render_moon/eifel.obj");
	auto moon = meshloader::loadObj("render_moon/moon.obj");
	auto sea = meshloader::loadObj("render_moon/sea.obj");
	auto seaLight = meshloader::loadObj("render_moon/sea-light.obj");
	auto background = meshloader::loadObj("render_moon/background.obj");

	auto seaMip = std::make_unique<MipMap>(loadPPM("render_moon/sea_disp.ppm"), 0);
	auto seaBump = std::make_unique<HeightMap>(seaMip.get());
	seaBump->setUVScale(666.0f, 666.0f);
	auto moonMip = std::make_unique<MipMap>(loadPPM("render_moon/moon_diffuse.ppm"), 16);
	auto moonTexture = std::make_unique<ImageTexture>(moonMip.get());
	auto roughness = std::make_unique<ColorTexture>(Spectrum(0.0f));
	auto roughness3 = std::make_unique<ColorTexture>(Spectrum(0.3f));
	auto white = std::make_unique<ColorTexture>(Spectrum(0.8f));
	auto bronze = std::make_unique<ColorTexture>(Spectrum(0.66f, 0.41f, 0.08f));
	auto orange = std::make_unique<ColorTexture>(Spectrum(1.0f, 0.11f, 0.08f));
	auto skyMip = std::make_unique<MipMap>(loadPPM("render_moon/sky.ppm"), 8);
	auto skyTexture = std::make_unique<ImageTexture>(skyMip.get());
	skyTexture->setUVScale(1.0f, 1.0f);

	auto seaMaterial = std::make_unique<GlassMaterial>(white.get(), roughness.get(), 1.33f);
	auto moonMaterial = std::make_unique<EmissionMaterial>(moonTexture.get(), 2.33f);
	auto orangeMaterial = std::make_unique<EmissionMaterial>(orange.get(), 40.0f);
	auto eifelMaterial = std::make_unique<MetalMaterial>(bronze.get(), roughness3.get());
	auto backgroundMaterial = std::make_unique<EmissionMaterial>(skyTexture.get(), 1.0f);

	seaMaterial->setBumpMap(seaBump.get());

	auto filter = std::make_unique<GaussianFilter>(0.75f);
	RenderSettings settings(2880, 1800, 64, samples, threads, filter.get());
	auto sampler = std::make_unique<StohasticSampler>(filter.get());
	auto cameraTransform =
		std::make_unique<Transformation>(
		transform::view(Point3(-53.0f, 33.0f, 2.4f), Point3(0.0f, 0.0f, 7.5f), Vector3(0.0f, 0.0f, 1.0f)));
	auto camera = std::make_unique<PerspectiveCamera>(cameraTransform.get(),
		settings.resolutionWidth, settings.resolutionHeight,
		0.25f, glm::length(Point3(-53.0f, 33.0f, 7.5f)), PI_OVER_FOUR - 0.3f);
	std::unique_ptr<Integrator> integrator = std::make_unique<PathIntegrator>();
	auto skyboxTexture = std::make_unique<ColorTexture>(Spectrum(0.1f));
	auto skybox = std::make_unique<Skybox>(skyboxTexture.get(), 1.0f, Transformation());
	Scene scene(skybox.get());

	scene.addEntity(eifel.get(), eifelMaterial.get());
	scene.addEntity(sea.get(), seaMaterial.get());
	scene.addLight(moon.get(), moonMaterial.get());
	scene.addLight(seaLight.get(), orangeMaterial.get());
	scene.addLight(background.get(), backgroundMaterial.get());

	Renderer renderer(&scene, camera.get(), integrator.get(), sampler.get(), settings);
	savePPM("moon-render.ppm", renderer.render());
	std::cout << "Press enter to exit" << std::endl;
	std::cin.get();
}

