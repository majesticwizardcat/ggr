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
#include "samplers/stohastic-sampler.h"
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

	std::shared_ptr<TriangleMesh> mirror(new Cube());
	std::shared_ptr<TriangleMesh> diffuse(new Cube());
	std::shared_ptr<TriangleMesh> light(new Cube());
	std::shared_ptr<TriangleMesh> glass(new Cube());
	std::shared_ptr<TriangleMesh> transparent(new Cube());
	std::shared_ptr<TriangleMesh> bluePlastic(new Cube());
	std::shared_ptr<TriangleMesh> greenPlastic(new Cube());
	std::shared_ptr<TriangleMesh> light2ndFloor(new Cube());

	mirror->transform(transform::translate(2.0f, 0.0f, 0.0f));
	diffuse->transform(transform::translate(-2.0f, 0.0f, 0.0f));
	glass->transform(transform::translate(0.0, -1.0f, 2.0f));
	transparent->transform(transform::translate(0.0f, -2.0f, 0.2f).combine(transform::scale(1.2f, 1.2f, 1.2f)));
	bluePlastic->transform(transform::translate(-2.0f, 0.0f, 3.0f));
	greenPlastic->transform(transform::translate(2.0f, 0.0f, 3.0f));
	light2ndFloor->transform(transform::translate(0.0f, 1.5f, 2.0f).combine(transform::scale(0.5f, 0.5f, 0.5f)));

	std::shared_ptr<TriangleMesh> plane(new Plane());
	std::shared_ptr<TriangleMesh> back(new Plane());
	plane->transform(transform::translate(0.0f, 0.0f, -0.5f).combine(transform::scale(15.0f, 15.0f, 1.0f)));
	back->transform(transform::translate(0.0f, 3.5f, 0.0f).combine(transform::rotate(PI_OVER_TWO, 0.0f, 0.0f))
		.combine(transform::scale(35.0f, 35.0f, 1.0f)));

	std::shared_ptr<MipMap> floorMip(new MipMap(CheckerboardImage(32), 8));

	std::shared_ptr<Texture> skyTexture(new ColorTexture(Spectrum(0.0f)));
	std::shared_ptr<Texture> redTexture(new ColorTexture(Spectrum(RGBColor(0.7f, 0.1f, 0.25f))));
	std::shared_ptr<Texture> whiteTexture(new ColorTexture(Spectrum(RGBColor(1.0f, 1.0f, 1.0f))));
	std::shared_ptr<Texture> blueTexture(new ColorTexture(Spectrum(RGBColor(0.0f, 0.1f, 0.85f))));
	std::shared_ptr<Texture> greenTexture(new ColorTexture(Spectrum(RGBColor(0.0f, 0.85f, 0.15f))));
	std::shared_ptr<Texture> roughness0(new ColorTexture(Spectrum(0.225f)));
	std::shared_ptr<Texture> roughness1(new ColorTexture(Spectrum(0.2f)));
	std::shared_ptr<Texture> roughness2(new ColorTexture(Spectrum(0.7f)));

	std::shared_ptr<Material> redMatte(new MatteMaterial(redTexture));
	std::shared_ptr<Material> metalMaterial(new MetalMaterial(whiteTexture, roughness0));
	std::shared_ptr<Material> mirrorMaterial(new MirrorMaterial(whiteTexture));
	std::shared_ptr<Material> glassMaterial(new GlassMaterial(whiteTexture, roughness0, 1.8f));
	std::shared_ptr<Material> transparentMaterial(new TransparentMaterial(whiteTexture, 1.5f));
	std::shared_ptr<Material> whitePlasticMaterial(new PlasticMaterial(whiteTexture, roughness0, 0.575f));
	std::shared_ptr<Material> bluePlasticMaterial(new PlasticMaterial(blueTexture, roughness1));
	std::shared_ptr<Material> greenPlasticMaterial(new PlasticMaterial(greenTexture, roughness2));
	std::shared_ptr<EmissionMaterial> emissionMat(new EmissionMaterial(whiteTexture, 3.5f));

	RenderSettings settings(500, 500, 16, samples, threads, std::shared_ptr<Filter>(new GaussianFilter(1.5f, 1.5f, 2.0f)));
	std::unique_ptr<Sampler> sampler(new StohasticSampler(settings.resolutionWidth, settings.resolutionHeight));
	std::unique_ptr<Camera> camera(new PerspectiveCamera(std::shared_ptr<Transformation>(new Transformation(
		transform::lookAt(Point3(0.0f, -5.0f, 1.5f), Point3(0.0f, 0.0f, 1.5f), Vector3(0.0f, 0.0f, 1.0f)))),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_TWO));
	std::unique_ptr<Integrator> integrator(new PathIntegrator());
	std::shared_ptr<Skybox> skybox(new Skybox(skyTexture));
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
}

