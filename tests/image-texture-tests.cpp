#include "integrators/direct-lighting-integrator.h"
#include "entities/entity.h"
#include "entities/light-entity.h"
#include "meshes/cube.h"
#include "meshes/plane.h"
#include "primitives/transformation.h"
#include "tools/constants.h"
#include "materials/matte-material.h"
#include "materials/emission-material.h"
#include "samplers/stohastic-sampler.h"
#include "cameras/perspective-camera.h"
#include "textures/color-texture.h"
#include "textures/image-texture.h"
#include "film/film.h"
#include "images/image-io.h"
#include "images/checkerboard-image.h"
#include "core/scene.h"
#include "core/renderer.h"
#include "core/render-settings.h"
#include "filters/box-filter.h"

#include <iostream>
#include <memory>

int main() {
	std::shared_ptr<TriangleMesh> plane(new Plane());
	std::shared_ptr<TriangleMesh> light(new Cube());
	plane->transform(transform::translate(0.0f, 0.0f, -5.0f).combine(transform::scale(10.0f, 10.0f, 1.0f)));
	light->transform(transform::translate(0.0f, 0.0f, 2.0f).combine(transform::scale(3.0f, 3.0f, 1.0f)));

	std::shared_ptr<MipMap> mipmap(new MipMap(CheckerboardImage(8), 4));
	std::shared_ptr<Texture> image(new ImageTexture(mipmap));
	std::shared_ptr<Texture> whiteTexture(new ColorTexture(Spectrum(RGBColor(1.0f, 1.0f, 1.0f))));

	std::shared_ptr<Material> matte(new MatteMaterial(image));
	std::shared_ptr<EmissionMaterial> emissionMat(new EmissionMaterial(whiteTexture, 13.5f));

	RenderSettings settings(500, 500, 16, 8, 1, std::shared_ptr<Filter>(new BoxFilter(0.5f, 0.5f)));
	std::unique_ptr<Sampler> sampler(new StohasticSampler(settings.resolutionWidth, settings.resolutionHeight));
	std::unique_ptr<Camera> camera(new PerspectiveCamera(std::shared_ptr<Transformation>(new Transformation(
		transform::lookAt(Point3(0.0f, 0.0f, 0.0f), Point3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO));
	std::unique_ptr<Integrator> integrator(new DirectLightingIntegrator());
	std::shared_ptr<Skybox> skybox(
		new Skybox(std::shared_ptr<Texture>(new ColorTexture(Spectrum(RGBColor(0.0f, 0.0f, 0.0f))))));
	Scene scene(skybox);

	scene.addEntity(plane, matte);
	scene.addLight(light, emissionMat);

	Renderer renderer(scene, camera, integrator, sampler, settings);
	savePPM("image-texture-test.ppm", renderer.render());
}

