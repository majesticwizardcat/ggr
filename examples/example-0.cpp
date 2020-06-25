#include "integrators/path-integrator.h"
#include "integrators/direct-lighting-integrator.h"
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
#include "film/film.h"
#include "images/image-io.h"
#include "images/mipmap.h"
#include "images/checkerboard-image.h"
#include "core/scene.h"
#include "core/renderer.h"
#include "core/render-settings.h"
#include "filters/gaussian-filter.h"

#include <iostream>
#include <memory>

const int DEFAULT_SAMPLES = 32;
const int DEFAULT_THREADS = 4;

int main(int args, char** argv) {
	int samples = DEFAULT_SAMPLES;
	int threads = DEFAULT_THREADS;
	if (args > 2) {
		samples = atoi(argv[1]);
		threads = atoi(argv[2]);
	}

	auto mirror = std::make_shared<Cube>();
	auto diffuse = std::make_shared<Cube>();
	auto light = std::make_shared<Cube>();
	auto glass = std::make_shared<Cube>();
	auto transparent = std::make_shared<Cube>();
	auto bluePlastic = std::make_shared<Cube>();
	auto greenPlastic = std::make_shared<Cube>();
	auto light2ndFloor = std::make_shared<Cube>();

	mirror->transform(transform::translate(2.0f, 0.0f, 0.0f));
	diffuse->transform(transform::translate(-2.0f, 0.0f, 0.0f));
	glass->transform(transform::translate(0.0, -1.0f, 2.0f));
	transparent->transform(transform::translate(0.0f, -2.0f, 0.2f).combine(transform::scale(1.2f, 1.2f, 1.2f)));
	bluePlastic->transform(transform::translate(-2.0f, 0.0f, 3.0f));
	greenPlastic->transform(transform::translate(2.0f, 0.0f, 3.0f));
	light2ndFloor->transform(transform::translate(0.0f, 1.5f, 2.0f).combine(transform::scale(0.5f, 0.5f, 0.5f)));

	auto plane = std::make_shared<Plane>();
	auto back = std::make_shared<Plane>();
	plane->transform(transform::translate(0.0f, 0.0f, -0.5f).combine(transform::scale(15.0f, 15.0f, 1.0f)));
	back->transform(transform::translate(0.0f, 3.5f, 0.0f).combine(transform::rotate(PI_OVER_TWO, 0.0f, 0.0f))
		.combine(transform::scale(35.0f, 35.0f, 1.0f)));

	auto floorMip = std::make_shared<MipMap>(CheckerboardImage(32), 8);

	auto skyTexture = std::make_shared<ColorTexture>(Spectrum(0.0f));
	auto redTexture = std::make_shared<ColorTexture>(Spectrum(RGBColor(0.7f, 0.1f, 0.25f)));
	auto whiteTexture = std::make_shared<ColorTexture>(Spectrum(RGBColor(1.0f, 1.0f, 1.0f)));
	auto blueTexture = std::make_shared<ColorTexture>(Spectrum(RGBColor(0.0f, 0.1f, 0.85f)));
	auto greenTexture = std::make_shared<ColorTexture>(Spectrum(RGBColor(0.0f, 0.85f, 0.15f)));
	auto roughness0 = std::make_shared<ColorTexture>(Spectrum(0.225f));
	auto roughness1 = std::make_shared<ColorTexture>(Spectrum(0.2f));
	auto roughness2 = std::make_shared<ColorTexture>(Spectrum(0.7f));

	auto redMatte = std::make_shared<MatteMaterial>(redTexture);
	auto metalMaterial = std::make_shared<MetalMaterial>(whiteTexture, roughness0);
	auto mirrorMaterial = std::make_shared<MirrorMaterial>(whiteTexture);
	auto glassMaterial = std::make_shared<GlassMaterial>(whiteTexture, roughness0, 1.8f);
	auto transparentMaterial = std::make_shared<TransparentMaterial>(whiteTexture, 1.5f);
	auto whitePlasticMaterial = std::make_shared<PlasticMaterial>(whiteTexture, roughness1);
	auto bluePlasticMaterial = std::make_shared<PlasticMaterial>(blueTexture, roughness1);
	auto greenPlasticMaterial = std::make_shared<PlasticMaterial>(greenTexture, roughness1);
	auto emissionMat = std::make_shared<EmissionMaterial>(whiteTexture, 3.5f);

	RenderSettings settings(500, 500, 16, samples, threads, std::make_shared<GaussianFilter>(1.5f, 1.5f, 2.0f));
	std::unique_ptr<Sampler> sampler = std::make_unique<HaltonSampler>(settings.resolutionWidth, settings.resolutionHeight);
	std::unique_ptr<Camera> camera = std::make_unique<PerspectiveCamera>(std::make_shared<Transformation>(
		transform::lookAt(Point3(0.0f, -5.0f, 1.5f), Point3(0.0f, 0.0f, 1.5f), Vector3(0.0f, 0.0f, 1.0f))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO);
	std::unique_ptr<Integrator> integrator = std::make_unique<PathIntegrator>();
	auto skybox = std::make_shared<Skybox>(skyTexture);
	Scene scene(skybox);

	scene.addEntity(diffuse, redMatte);
	scene.addEntity(mirror, mirrorMaterial);
	scene.addEntity(plane, whitePlasticMaterial);
	scene.addEntity(back, metalMaterial);
	scene.addEntity(bluePlastic, bluePlasticMaterial);
	scene.addEntity(greenPlastic, greenPlasticMaterial);
	scene.addEntity(glass, glassMaterial);
	scene.addEntity(transparent, transparentMaterial);
	scene.addLight(light, emissionMat);
	scene.addLight(light2ndFloor, emissionMat);

	Renderer renderer(scene, camera, integrator, sampler, settings);
	savePPM("example-0-render.ppm", renderer.render());
	std::cin.get();
}

