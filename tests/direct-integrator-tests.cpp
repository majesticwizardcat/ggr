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
#include "samplers/stohastic-sampler.h"
#include "cameras/perspective-camera.h"
#include "textures/color-texture.h"
#include "film/film.h"
#include "images/image-io.h"
#include "core/scene.h"
#include "core/renderer.h"
#include "core/render-settings.h"
#include "filters/gaussian-filter.h"
#include "filters/box-filter.h"

#include <iostream>
#include <memory>

int main() {
	std::shared_ptr<Sphere> c(new Sphere());
	std::shared_ptr<Cube> light(new Cube());
	c->transform(transform::rotate(PI_OVER_TWO, PI_OVER_FOUR, 0.0f).combine(transform::scale(1.5f, 1.5f, 1.5f)));
	light->transform(transform::translate(5.5f, 0.5f, 1.0f).combine(transform::scale(3.5f, 3.5f, 3.5f)));
	std::shared_ptr<Plane> plane(new Plane());
	plane->transform(transform::translate(0.0f, 0.0f, -2.0f).combine(transform::scale(15.0f, 15.0f, 1.0f)));

	std::shared_ptr<Texture> colorTexture(new ColorTexture(Spectrum(RGBColor(0.2f, 0.6f, 0.1f))));
	std::shared_ptr<Texture> whiteTexture(new ColorTexture(Spectrum(RGBColor(1.0f, 1.0f, 1.0f))));
	std::shared_ptr<Texture> lightTexture(new ColorTexture(Spectrum(RGBColor(1.0f, 1.0f, 1.0f))));
	std::shared_ptr<Material> matteMat(new MatteMaterial(colorTexture));
	std::shared_ptr<Material> whiteMatte(new MatteMaterial(whiteTexture));
	std::shared_ptr<EmissionMaterial> emissionMat(new EmissionMaterial(lightTexture, 1.3f));

	RenderSettings settings(500, 500, 16, 16, 4, std::shared_ptr<Filter>(new BoxFilter(0.5f, 0.5f)));
	std::unique_ptr<Sampler> sampler(new StohasticSampler(settings.resolutionWidth, settings.resolutionHeight));
	std::unique_ptr<Camera> camera(new PerspectiveCamera(std::shared_ptr<Transformation>(new Transformation(
		transform::lookAt(Point3(0.75f, 2.0f, 5.0f), Point3(0.5f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 5.5f, PI_OVER_TWO));
	std::unique_ptr<Integrator> integrator(new DirectLightingIntegrator());
	std::shared_ptr<Skybox> skybox(
		new Skybox(std::shared_ptr<Texture>(new ColorTexture(Spectrum(0.0f)))));
	Scene scene(skybox);
	scene.addEntity(c, matteMat);
	scene.addEntity(plane, whiteMatte);
	scene.addLight(light, emissionMat);
	Renderer renderer(scene, camera, integrator, sampler, settings);
	savePPM("direct-integrator-test.ppm", renderer.render());
}

