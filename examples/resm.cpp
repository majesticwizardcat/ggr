#include "integrators/path-integrator.h"
#include "entities/entity.h"
#include "entities/light-entity.h"
#include "meshes/cube.h"
#include "meshes/plane.h"
#include "meshes/sphere.h"
#include "meshes/mesh-loader.h"
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
#include "filters/box-filter.h"
#include "samplers/stohastic-sampler.h"

#include <iostream>
#include <memory>

const int DEFAULT_SAMPLES = 16;
const int DEFAULT_THREADS = 4;

int main(int args, char** argv) {
	int samples = DEFAULT_SAMPLES;
	int threads = DEFAULT_THREADS;
	if (args > 2) {
		samples = atoi(argv[1]);
		threads = atoi(argv[2]);
	}
	auto floor = meshloader::loadObj("floor-out.obj");
	auto budha = meshloader::loadObj("budha-out.obj");
	auto bunny = meshloader::loadObj("bunny-out.obj");
	auto teapot = meshloader::loadObj("teapot-out.obj");
	auto suzzane = meshloader::loadObj("suzzane-out.obj");
	auto light = meshloader::loadObj("light-out.obj");

	Image sky = loadPPM("sky.ppm");
	auto skymip = std::make_unique<MipMap>(sky, 0);
	
	Image floorDif = loadPPM("diffuse.ppm");
	Image floorRough = loadPPM("roughness.ppm");
	Image floorHeight = loadPPM("height.ppm");
	auto floorDifMip = std::make_unique<MipMap>(floorDif, 0);
	auto floorRoughMip = std::make_unique<MipMap>(floorRough, 0);
	auto floorHeightMip = std::make_unique<MipMap>(floorHeight, 0);
	auto floorDifTexture = std::make_unique<ImageTexture>(floorDifMip.get());
	auto floorRoughTexture = std::make_unique<ImageTexture>(floorRoughMip.get());
	floorDifTexture->setUVScale(5.0f, 5.0f);
	floorRoughTexture->setUVScale(5.0f, 5.0f);

	auto lightTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(1.0f, 0.79f, 0.485f)));
	auto skyTexture = std::make_unique<ImageTexture>(skymip.get());
	auto whiteTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.8f, 0.8f, 0.8f)));
	auto rubyTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.878f, 0.067f, 0.373f)));
	auto saphireTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.059f, 0.322f, 0.729f)));
	auto emeraldTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.314f, 0.784f, 0.471f)));
	auto amberTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(1.0f, 0.749f, 0.0f)));
	auto roughness = std::make_unique<ColorTexture>(Spectrum(0.175f));
	auto roughnessGlass = std::make_unique<ColorTexture>(Spectrum(0.0f));
	auto roughnessRuby = std::make_unique<ColorTexture>(Spectrum(0.25f));

	auto emissionMat = std::make_unique<EmissionMaterial>(lightTexture.get(), 500.0f);
	auto metal = std::make_unique<MetalMaterial>(whiteTexture.get(), roughness.get());
	auto floorMat = std::make_unique<PlasticMaterial>(floorDifTexture.get(), floorRoughTexture.get(), 0.7f);
	auto ruby = std::make_unique<GlassMaterial>(rubyTexture.get(), roughnessRuby.get(), 1.757f);
	auto saphire = std::make_unique<GlassMaterial>(saphireTexture.get(), roughnessGlass.get(), 1.750f);
	auto emerald = std::make_unique<GlassMaterial>(emeraldTexture.get(), roughnessGlass.get(), 1.56f);
	auto amber = std::make_unique<GlassMaterial>(amberTexture.get(), roughnessGlass.get(), 1.54f);

	auto bump = std::make_unique<HeightMap>(floorHeightMip.get());
	bump->setUVScale(5.0f, 5.0f);
	floorMat->setBumpMap(bump.get());

	auto filter = std::make_unique<BoxFilter>(0.75f);
	RenderSettings settings(1920, 1080, 16, samples, threads, filter.get());
	auto sampler = std::make_unique<StohasticSampler>(filter.get());
	auto cameraTransform =
		std::make_unique<Transformation>(
		transform::view(Point3(0.0f, -7.0f, 2.5f), Point3(0.175f, 0.0f, 0.4f), Vector3(0.0f, 0.0f, 1.0f)));
	auto camera = std::make_unique<PerspectiveCamera>(cameraTransform.get(),
		settings.resolutionWidth, settings.resolutionHeight,
		0.04f, 7.2f, 0.35f);
	std::unique_ptr<Integrator> integrator = std::make_unique<PathIntegrator>();
	auto skybox = std::make_unique<Skybox>(skyTexture.get(), 5.75f,
		transform::rotateZ(0.475f + PI));
	Scene scene(skybox.get());

	scene.addLight(light.get(), emissionMat.get());
	scene.addEntity(floor.get(), floorMat.get());
	scene.addEntity(budha.get(), ruby.get());
	scene.addEntity(bunny.get(), saphire.get());
	scene.addEntity(teapot.get(), metal.get());
	scene.addEntity(suzzane.get(), emerald.get());

	Renderer renderer(&scene, camera.get(), integrator.get(), sampler.get(), settings);
	savePPM("glass-render.ppm", renderer.render());
	std::cout << "Press enter to exit" << std::endl;
	std::cin.get();
}
