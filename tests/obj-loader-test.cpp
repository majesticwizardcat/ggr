#include "integrators/path-integrator.h"
#include "entities/entity.h"
#include "entities/light-entity.h"
#include "meshes/plane.h"
#include "meshes/cube.h"
#include "meshes/mesh-loader.h"
#include "primitives/transformation.h"
#include "tools/constants.h"
#include "materials/matte-material.h"
#include "materials/emission-material.h"
#include "samplers/halton-sampler.h"
#include "cameras/perspective-camera.h"
#include "textures/color-texture.h"
#include "film/film.h"
#include "images/image-io.h"
#include "core/scene.h"
#include "core/renderer.h"
#include "core/render-settings.h"
#include "filters/box-filter.h"
#include "tools/constants.h"

#include <iostream>
#include <memory>

int main(int args, char** argv) {
	std::shared_ptr<TriangleMesh> monkey = std::move(meshloader::loadObj("monkey.obj"));

	auto floor = std::make_shared<Plane>();
	auto l0 = std::make_shared<Cube>();
	auto l1 = std::make_shared<Cube>();
	auto l2 = std::make_shared<Cube>();
	auto l3 = std::make_shared<Cube>();

	floor->transform(transform::translate(0.0f, 0.0f, 3.5f).combine(
		transform::scale(15.0f, 15.0f, 1.0f).combine(transform::rotate(PI, 0.0f, 0.0f))));
	monkey->transform(transform::rotate(0.0f, PI, 0.0f));
	l0->transform(transform::translate(5.0f, 5.0f, -2.0f));
	l1->transform(transform::translate(-5.0f, 5.0f, -2.0f));
	l2->transform(transform::translate(-5.0f, -5.0f, -2.0f));
	l3->transform(transform::translate(5.0f, -5.0f, -2.0f));

	auto whiteTexture = std::make_shared<ColorTexture>(Spectrum(1.0f));
	auto grayTexture = std::make_shared<ColorTexture>(Spectrum(0.75f));
	auto redTexture = std::make_shared<ColorTexture>(Spectrum(1.0f, 0.0f, 0.0f));
	auto greenTexture = std::make_shared<ColorTexture>(Spectrum(0.0f, 1.0f, 0.0f));
	auto blueTexture = std::make_shared<ColorTexture>(Spectrum(0.0f, 0.0f, 1.0f));
	auto yellowTexture = std::make_shared<ColorTexture>(Spectrum(1.0f, 1.0f, 0.0f));

	auto whiteMatte = std::make_shared<MatteMaterial>(whiteTexture);
	auto grayMatte = std::make_shared<MatteMaterial>(grayTexture);
	auto emissionMatRed = std::make_shared<EmissionMaterial>(redTexture, 17.0f);
	auto emissionMatGreen = std::make_shared<EmissionMaterial>(greenTexture, 17.0f);
	auto emissionMatBlue = std::make_shared<EmissionMaterial>(blueTexture, 17.0f);
	auto emissionMatYellow = std::make_shared<EmissionMaterial>(yellowTexture, 17.0f);

	RenderSettings settings(500, 500, 16, 32, 4, std::make_shared<BoxFilter>(1.0f, 1.0f));
	std::unique_ptr<Sampler> sampler = std::make_unique<HaltonSampler>(settings.resolutionWidth, settings.resolutionHeight);
	std::unique_ptr<Camera> camera = std::make_unique<PerspectiveCamera>(std::make_shared<Transformation>(
		transform::view(Point3(0.0f, 0.0f, -2.5f), Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO);
	std::unique_ptr<Integrator> integrator = std::make_unique<PathIntegrator>();
	auto blackTexture = std::make_shared<ColorTexture>(Spectrum(0.0f));
	auto skybox = std::make_shared<Skybox>(blackTexture);
	Scene scene(skybox);

	scene.addEntity(floor, grayMatte);
	scene.addEntity(monkey, whiteMatte);
	scene.addLight(l0, emissionMatRed);
	scene.addLight(l1, emissionMatGreen);
	scene.addLight(l2, emissionMatBlue);
	scene.addLight(l3, emissionMatYellow);

	Renderer renderer(scene, camera, integrator, sampler, settings);
	savePPM("obj-loader-test.ppm", renderer.render());
}

