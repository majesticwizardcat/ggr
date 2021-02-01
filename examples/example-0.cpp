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
#include "materials/mirror-material.h"
#include "materials/transparent-material.h"
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

#include <iostream>
#include <memory>

const int DEFAULT_SAMPLES = 64;
const int DEFAULT_THREADS = 4;

int main(int args, char** argv) {
	int samples = DEFAULT_SAMPLES;
	int threads = DEFAULT_THREADS;
	if (args > 2) {
		samples = atoi(argv[1]);
		threads = atoi(argv[2]);
	}

	auto mirror = std::make_unique<Cube>();
	auto diffuse = std::make_unique<Cube>();
	auto light = std::make_unique<Cube>();
	auto transparent = std::make_unique<Cube>();
	auto bluePlastic = std::make_unique<Cube>();
	auto greenPlastic = std::make_unique<Cube>();
	auto light2ndFloor = std::make_unique<Cube>();

	mirror->transform(transform::translate(2.0f, 0.0f, 0.0f));
	diffuse->transform(transform::translate(-2.0f, 0.0f, 0.0f));
	transparent->transform(transform::translate(0.0f, -2.0f, 0.2f).combine(transform::scale(1.2f, 1.2f, 1.2f)));
	bluePlastic->transform(transform::translate(-2.0f, 0.0f, 3.0f));
	greenPlastic->transform(transform::translate(2.0f, 0.0f, 3.0f));
	light2ndFloor->transform(transform::translate(0.0f, 1.5f, 2.0f).combine(transform::scale(0.5f, 0.5f, 0.5f)));

	auto plane = std::make_unique<Plane>();
	auto back = std::make_unique<Plane>();
	plane->transform(transform::translate(0.0f, 0.0f, -0.5f).combine(transform::scale(15.0f, 15.0f, 1.0f)));
	back->transform(transform::translate(0.0f, 3.5f, 0.0f).combine(transform::rotate(PI_OVER_TWO, 0.0f, 0.0f))
		.combine(transform::scale(35.0f, 35.0f, 1.0f)));

	CheckerboardImage checker(24);
	auto floorMip = std::make_unique<MipMap>(checker, 8);
	auto floorHeightMip = std::make_unique<MipMap>(checker.generateHeightMap(), 8);

	auto floorTexture = std::make_unique<ImageTexture>(floorMip.get());
	auto skyTexture = std::make_unique<ColorTexture>(Spectrum(0.0f));
	auto redTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.7f, 0.1f, 0.25f)));
	auto whiteTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(1.0f, 1.0f, 1.0f)));
	auto blueTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.0f, 0.1f, 0.85f)));
	auto greenTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.0f, 0.85f, 0.15f)));
	auto roughness0 = std::make_unique<ColorTexture>(Spectrum(0.225f));
	auto roughness1 = std::make_unique<ColorTexture>(Spectrum(0.2f));

	auto floorBumpMap = std::make_unique<HeightMap>(floorHeightMip.get());
	floorTexture->setUVScale(1.5f, 1.5f);
	floorBumpMap->setUVScale(1.5f, 1.5f);

	auto redMatte = std::make_unique<MatteMaterial>(redTexture.get());
	auto metalMaterial = std::make_unique<MetalMaterial>(whiteTexture.get(), roughness0.get());
	auto mirrorMaterial = std::make_unique<MirrorMaterial>(whiteTexture.get());
	auto glassMaterial = std::make_unique<GlassMaterial>(whiteTexture.get(), roughness0.get(), 1.33);
	auto bluePlasticMaterial = std::make_unique<PlasticMaterial>(blueTexture.get(), roughness1.get(), 0.5f);
	auto greenPlasticMaterial = std::make_unique<PlasticMaterial>(greenTexture.get(), roughness1.get());
	auto floorMaterial = std::make_unique<PlasticMaterial>(floorTexture.get(), roughness1.get(), 0.5f);
	auto emissionMat = std::make_unique<EmissionMaterial>(whiteTexture.get(), 3.5f);

	floorMaterial->setBumpMap(floorBumpMap.get());

	auto filter = std::make_unique<GaussianFilter>(1.75f);
	RenderSettings settings(500, 500, 64, samples, threads, filter.get());
	auto sampler = std::make_unique<StohasticSampler>(filter.get());
	auto cameraTransform =
		std::make_unique<Transformation>(
		transform::view(Point3(0.0f, -5.0f, 1.5f), Point3(0.0f, 0.0f, 1.5f), Vector3(0.0f, 0.0f, 1.0f)));
	auto camera = std::make_unique<PerspectiveCamera>(cameraTransform.get(),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO);
	std::unique_ptr<Integrator> integrator = std::make_unique<PathIntegrator>();
	auto skybox = std::make_unique<Skybox>(skyTexture.get(), 1.0f, Transformation());
	Scene scene(skybox.get());

	scene.addEntity(diffuse.get(), redMatte.get());
	scene.addEntity(mirror.get(), mirrorMaterial.get());
	scene.addEntity(plane.get(), floorMaterial.get());
	scene.addEntity(back.get(), metalMaterial.get());
	scene.addEntity(bluePlastic.get(), bluePlasticMaterial.get());
	scene.addEntity(greenPlastic.get(), greenPlasticMaterial.get());
	scene.addEntity(transparent.get(), glassMaterial.get());
	scene.addLight(light.get(), emissionMat.get());
	scene.addLight(light2ndFloor.get(), emissionMat.get());

	Renderer renderer(&scene, camera.get(), integrator.get(), sampler.get(), settings);
	savePPM("example-0-render.ppm", renderer.render());
	std::cout << "Press enter to exit" << std::endl;
	std::cin.get();
}

