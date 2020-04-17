#include "integrators/direct-lighting-integrator.h"
#include "meshes/plane.h"
#include "samplers/stohastic-sampler.h"
#include "cameras/perspective-camera.h"
#include "images/mipmap.h"
#include "images/checkerboard-image.h"
#include "images/image-io.h"
#include "core/scene.h"
#include "core/renderer.h"
#include "core/render-settings.h"
#include "tools/constants.h"
#include "textures/image-texture.h"
#include "textures/color-texture.h"
#include "materials/matte-material.h"
#include "filters/box-filter.h"

#include <memory>
#include <iostream>

int main() {
	std::shared_ptr<TriangleMesh> floor(new Plane());
	floor->transform(transform::translate(0.0f, 0.0f, 9.0f).combine(
		transform::rotate(-PI_OVER_TWO, 0.0f, 0.0f).combine(
		transform::scale(20.0f, 20.0f, 1.0f))));

	std::shared_ptr<MipMap> mipTri(new MipMap(CheckerboardImage(64), 0));
	std::shared_ptr<MipMap> mipAni4(new MipMap(CheckerboardImage(64), 4));
	std::shared_ptr<MipMap> mipAni16(new MipMap(CheckerboardImage(64), 16));

	std::shared_ptr<Texture> texTri(new ImageTexture(mipTri));
	std::shared_ptr<Texture> texAni4(new ImageTexture(mipAni4));
	std::shared_ptr<Texture> texAni16(new ImageTexture(mipAni16));
	std::shared_ptr<Texture> skyboxTexture(new ColorTexture(Spectrum(1.0f)));

	std::shared_ptr<Material> matTri(new MatteMaterial(texTri));
	std::shared_ptr<Material> matAni4(new MatteMaterial(texAni4));
	std::shared_ptr<Material> matAni16(new MatteMaterial(texAni16));

	RenderSettings settings(500, 500, 16, 16, 4, std::shared_ptr<Filter>(new BoxFilter(0.5f, 0.5f)));
	std::shared_ptr<Skybox> skybox(new Skybox(skyboxTexture));

	Scene sceneTri(skybox);
	sceneTri.addEntity(floor, matTri);

	Scene sceneAni4(skybox);
	sceneAni4.addEntity(floor, matAni4);

	Scene sceneAni16(skybox);
	sceneAni16.addEntity(floor, matAni16);

	std::cout << "Rendering trilinear: " << std::endl;
	std::unique_ptr<Sampler> sampler(new StohasticSampler(settings.resolutionWidth, settings.resolutionHeight));
	std::unique_ptr<Camera> camera(new PerspectiveCamera(std::shared_ptr<Transformation>(new Transformation(
		transform::lookAt(Point3(0.0f, 1.0f, 0.0f), Point3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO));
	std::unique_ptr<Integrator> integrator(new DirectLightingIntegrator());
	Renderer rendererTri(sceneTri, camera, integrator, sampler, settings);
	savePPM("filter-tri-test.ppm", rendererTri.render());

	std::cout << "Rendering anisotropicX4: " << std::endl;
	sampler = std::unique_ptr<Sampler>(new StohasticSampler(settings.resolutionWidth, settings.resolutionHeight));
	camera = std::unique_ptr<Camera>(new PerspectiveCamera(std::shared_ptr<Transformation>(new Transformation(
		transform::lookAt(Point3(0.0f, 1.0f, 0.0f), Point3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO));
	integrator = std::unique_ptr<Integrator>(new DirectLightingIntegrator());
	Renderer rendererAni4(sceneAni4, camera, integrator, sampler, settings);
	savePPM("filter-ani4-test.ppm", rendererAni4.render());

	std::cout << "Rendering anisotropicX16: " << std::endl;
	sampler = std::unique_ptr<Sampler>(new StohasticSampler(settings.resolutionWidth, settings.resolutionHeight));
	camera = std::unique_ptr<Camera>(new PerspectiveCamera(std::shared_ptr<Transformation>(new Transformation(
		transform::lookAt(Point3(0.0f, 1.0f, 0.0f), Point3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO));
	integrator = std::unique_ptr<Integrator>(new DirectLightingIntegrator());
	Renderer rendererAni16(sceneAni16, camera, integrator, sampler, settings);
	savePPM("filter-ani16-test.ppm", rendererAni16.render());
}

