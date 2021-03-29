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
#include "materials/blend-material.h"
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
#include "filters/box-filter.h"
#include "filters/gaussian-filter.h"
#include "samplers/stohastic-sampler.h"
#include "meshes/mesh-loader.h"

#include <iostream>
#include <memory>

const int DEFAULT_SAMPLES = 2560;
const int DEFAULT_THREADS = 4;

int main(int args, char** argv) {
	int samples = DEFAULT_SAMPLES;
	int threads = DEFAULT_THREADS;
	if (args > 2) {
		samples = atoi(argv[1]);
		threads = atoi(argv[2]);
	}

	auto mesh000 = meshloader::loadObj("render/mesh000.obj");
	auto mesh001 = meshloader::loadObj("render/mesh001.obj");
	auto mesh002 = meshloader::loadObj("render/mesh002.obj");
	auto mesh003 = meshloader::loadObj("render/mesh003.obj");
	auto mesh004 = meshloader::loadObj("render/mesh004.obj");
	auto mesh005 = meshloader::loadObj("render/mesh005.obj");
	auto mesh006 = meshloader::loadObj("render/mesh006.obj");
	auto mesh012 = meshloader::loadObj("render/mesh012.obj");
	auto mesh013 = meshloader::loadObj("render/mesh013.obj");
	auto mesh014 = meshloader::loadObj("render/mesh014.obj");
	auto mesh015 = meshloader::loadObj("render/mesh015.obj");
	auto mesh016 = meshloader::loadObj("render/mesh016.obj");
	auto mesh019 = meshloader::loadObj("render/mesh019.obj");
	auto mesh021 = meshloader::loadObj("render/mesh021.obj");
	auto mesh022 = meshloader::loadObj("render/mesh022.obj");
	auto mesh023 = meshloader::loadObj("render/mesh023.obj");
	auto mesh024 = meshloader::loadObj("render/mesh024.obj");
	auto mesh027 = meshloader::loadObj("render/mesh027.obj");
	auto mesh028 = meshloader::loadObj("render/mesh028.obj");
	auto mesh029 = meshloader::loadObj("render/mesh029.obj");
	auto mesh030 = meshloader::loadObj("render/mesh030.obj");
	auto mesh031 = meshloader::loadObj("render/mesh031.obj");
	auto mesh033 = meshloader::loadObj("render/mesh033.obj");
	auto mesh034 = meshloader::loadObj("render/mesh034.obj");
	auto mesh037 = meshloader::loadObj("render/mesh037.obj");
	auto mesh038 = meshloader::loadObj("render/mesh038.obj");
	auto mesh039 = meshloader::loadObj("render/mesh039.obj");
	auto mesh040 = meshloader::loadObj("render/mesh040.obj");
	auto mesh042 = meshloader::loadObj("render/mesh042.obj");
	auto mesh043 = meshloader::loadObj("render/mesh043.obj");
	auto mesh044 = meshloader::loadObj("render/mesh044.obj");
	auto mesh045 = meshloader::loadObj("render/mesh045.obj");
	auto mesh046 = meshloader::loadObj("render/mesh046.obj");
	auto mesh047 = meshloader::loadObj("render/mesh047.obj");
	auto mesh048 = meshloader::loadObj("render/mesh048.obj");
	auto mesh050 = meshloader::loadObj("render/mesh050.obj");
	auto mesh051 = meshloader::loadObj("render/mesh051.obj");
	auto mesh054 = meshloader::loadObj("render/mesh054.obj");
	auto mesh055 = meshloader::loadObj("render/mesh055.obj");
	auto mesh056 = meshloader::loadObj("render/mesh056.obj");
	auto mesh057 = meshloader::loadObj("render/mesh057.obj");
	auto mesh059 = meshloader::loadObj("render/mesh059.obj");
	auto mesh061 = meshloader::loadObj("render/mesh061.obj");
	auto mesh062 = meshloader::loadObj("render/mesh062.obj");
	auto mesh063 = meshloader::loadObj("render/mesh063.obj");
	auto mesh064 = meshloader::loadObj("render/mesh064.obj");
	auto plant1_stem1 = meshloader::loadObj("render/plant1_stem1.obj");
	auto plant1_stem2 = meshloader::loadObj("render/plant1_stem2.obj");
	auto plant1_ground = meshloader::loadObj("render/plant1_ground.obj");
	auto plant1_pot = meshloader::loadObj("render/plant1_pot.obj");
	auto plant1_leaves = meshloader::loadObj("render/plant1_leaves.obj");
	auto plant2_stem1 = meshloader::loadObj("render/plant2_stem1.obj");
	auto plant2_stem2 = meshloader::loadObj("render/plant2_stem2.obj");
	auto plant2_ground = meshloader::loadObj("render/plant2_ground.obj");
	auto plant2_pot = meshloader::loadObj("render/plant2_pot.obj");
	auto plant2_leaves = meshloader::loadObj("render/plant2_leaves.obj");

	auto tableMip = std::make_unique<MipMap>(loadPPM("render/table.ppm"), 0);
	auto floorMip = std::make_unique<MipMap>(loadPPM("render/floor.ppm"), 0);
	auto catMip = std::make_unique<MipMap>(loadPPM("render/cat.ppm"), 0);
	auto groundDiff = std::make_unique<MipMap>(loadPPM("render/ground_diff.ppm"), 0);
	auto groundHeight = std::make_unique<MipMap>(loadPPM("render/ground_bmp.ppm"), 0);
	auto leafDiff = std::make_unique<MipMap>(loadPPM("render/leaf_diff.ppm"), 0);
	auto leafHeight = std::make_unique<MipMap>(loadPPM("render/leaf_bmp.ppm"), 0);
	auto stem1 = std::make_unique<MipMap>(loadPPM("render/stem1.ppm"), 0);
	auto stem2 = std::make_unique<MipMap>(loadPPM("render/stem2.ppm"), 0);

	auto stem1Texture = std::make_unique<ImageTexture>(stem1.get());
	auto stem2Texture = std::make_unique<ImageTexture>(stem2.get());
	auto groundDiffTexture = std::make_unique<ImageTexture>(groundDiff.get());
	auto leafDiffTexture = std::make_unique<ImageTexture>(leafDiff.get());
	auto tableTexture = std::make_unique<ImageTexture>(tableMip.get());
	auto floorTexture = std::make_unique<ImageTexture>(floorMip.get());
	auto whiteTexture = std::make_unique<ColorTexture>(Spectrum(1.0f));
	auto white4 = std::make_unique<ColorTexture>(Spectrum(0.4f));
	auto roughness25 = std::make_unique<ColorTexture>(Spectrum(0.25f));
	auto roughness0 = std::make_unique<ColorTexture>(Spectrum(0.0f));
	auto roughness1 = std::make_unique<ColorTexture>(Spectrum(0.1f));
	auto roughness15 = std::make_unique<ColorTexture>(Spectrum(0.15f));
	auto roughness3 = std::make_unique<ColorTexture>(Spectrum(0.3f));
	auto roughness4 = std::make_unique<ColorTexture>(Spectrum(0.4f));
	auto red = std::make_unique<ColorTexture>(Spectrum(RGBColor(1.0f, 0.1f, 0.1f)));
	auto olive = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.26f, 0.25f, 0.14f)));
	auto white57 = std::make_unique<ColorTexture>(Spectrum(0.57f));
	auto black01 = std::make_unique<ColorTexture>(Spectrum(RGBColor(0.1f, 0.1f, 0.1f)));
	auto white8 = std::make_unique<ColorTexture>(Spectrum(0.8f));
	auto white5 = std::make_unique<ColorTexture>(Spectrum(0.5f));
	auto catTex = std::make_unique<ImageTexture>(catMip.get());
	auto white9 = std::make_unique<ColorTexture>(Spectrum(0.9f));

	auto groundHeightMap = std::make_unique<HeightMap>(groundHeight.get());
	auto leafHeightMap = std::make_unique<HeightMap>(leafHeight.get());

	auto pot = std::make_unique<PlasticMaterial>(white8.get(), roughness15.get(), 0.2f);
	auto stem1Mat = std::make_unique<MatteMaterial>(stem1Texture.get());
	auto stem2Mat = std::make_unique<MatteMaterial>(stem2Texture.get());
	auto ground = std::make_unique<MatteMaterial>(groundDiffTexture.get());
	ground->setBumpMap(groundHeightMap.get());
	auto leaf = std::make_unique<MatteMaterial>(leafDiffTexture.get());
	leaf->setBumpMap(leafHeightMap.get());
	auto whiteMatte = std::make_unique<MatteMaterial>(whiteTexture.get());
	auto tableWood = std::make_unique<BlendMaterial>(tableTexture.get(), white4.get(), roughness25.get());
	auto redCap = std::make_unique<MatteMaterial>(red.get());
	auto glass = std::make_unique<GlassMaterial>(whiteTexture.get(), roughness0.get(), 1.5f);
	auto paintingBack = std::make_unique<MatteMaterial>(olive.get());
	auto white57Matte = std::make_unique<MatteMaterial>(white57.get());
	auto horse = std::make_unique<MetalMaterial>(black01.get(), roughness1.get());
	auto metal = std::make_unique<MetalMaterial>(white8.get(), roughness15.get());
	auto floorWood = std::make_unique<BlendMaterial>(floorTexture.get(), white5.get(), roughness3.get());
	auto blackMatte = std::make_unique<MatteMaterial>(black01.get());
	auto catMat = std::make_unique<MatteMaterial>(catTex.get());
	auto panelling = std::make_unique<PlasticMaterial>(whiteTexture.get(), roughness4.get(), 0.2f);
	auto white9Matte = std::make_unique<MatteMaterial>(white9.get());
	auto mirror = std::make_unique<MetalMaterial>(whiteTexture.get(), roughness0.get());
	auto fireplace = std::make_unique<MetalMaterial>(black01.get(), roughness1.get());

	auto filter = std::make_unique<GaussianFilter>(0.8f);
	RenderSettings settings(1920, 1080, 64, samples, threads, filter.get());
	auto sampler = std::make_unique<StohasticSampler>(filter.get());
	auto cameraTransform =
		std::make_unique<Transformation>(
		transform::view(Point3(17.4f, 3.26f, -8.64f), Point3(6.8f, 3.5f, -6.1f), Vector3(0.0f, 1.0f, 0.0f)));
	auto camera = std::make_unique<PerspectiveCamera>(cameraTransform.get(),
		settings.resolutionWidth, settings.resolutionHeight,
		0.0f, 0.0f, PI_OVER_THREE);
	std::unique_ptr<Integrator> integrator = std::make_unique<PathIntegrator>();

	auto skyTexture = std::make_unique<ColorTexture>(Spectrum(RGBColor(1.0f, 0.87f, 0.529f)));
	auto skybox = std::make_unique<Skybox>(skyTexture.get(), 2.3f, Transformation());
	Scene scene(skybox.get());

	scene.addEntity(mesh000.get(), tableWood.get());
	scene.addEntity(mesh001.get(), redCap.get());
	scene.addEntity(mesh002.get(), glass.get());
	scene.addEntity(mesh003.get(), tableWood.get());
	scene.addEntity(mesh004.get(), tableWood.get());
	scene.addEntity(mesh005.get(), paintingBack.get());
	scene.addEntity(mesh006.get(), white57Matte.get());
	scene.addEntity(mesh012.get(), horse.get());
	scene.addEntity(mesh013.get(), whiteMatte.get());
	scene.addEntity(mesh014.get(), whiteMatte.get());
	scene.addEntity(mesh015.get(), metal.get());
	scene.addEntity(mesh016.get(), metal.get());
	scene.addEntity(mesh019.get(), floorWood.get());
	scene.addEntity(mesh021.get(), blackMatte.get());
	scene.addEntity(mesh022.get(), metal.get());
	scene.addEntity(mesh023.get(), catMat.get());
	scene.addEntity(mesh024.get(), panelling.get());
	scene.addEntity(mesh027.get(), panelling.get());
	scene.addEntity(mesh028.get(), white57Matte.get());
	scene.addEntity(mesh029.get(), panelling.get());
	scene.addEntity(mesh030.get(), glass.get());
	scene.addEntity(mesh031.get(), tableWood.get());
	scene.addEntity(mesh033.get(), panelling.get());
	scene.addEntity(mesh034.get(), whiteMatte.get());
	scene.addEntity(mesh037.get(), tableWood.get());
	scene.addEntity(mesh038.get(), panelling.get());
	scene.addEntity(mesh039.get(), panelling.get());
	scene.addEntity(mesh040.get(), white9Matte.get());
	scene.addEntity(mesh042.get(), white57Matte.get());
	scene.addEntity(mesh043.get(), panelling.get());
	scene.addEntity(mesh044.get(), glass.get());
	scene.addEntity(mesh045.get(), mirror.get());
	scene.addEntity(mesh046.get(), panelling.get());
	scene.addEntity(mesh047.get(), whiteMatte.get());
	scene.addEntity(mesh048.get(), panelling.get());
	scene.addEntity(mesh050.get(), panelling.get());
	scene.addEntity(mesh051.get(), panelling.get());
	scene.addEntity(mesh054.get(), blackMatte.get());
	scene.addEntity(mesh055.get(), panelling.get());
	scene.addEntity(mesh056.get(), fireplace.get());
	scene.addEntity(mesh057.get(), white57Matte.get());
	scene.addEntity(mesh059.get(), blackMatte.get());
	scene.addEntity(mesh061.get(), panelling.get());
	scene.addEntity(mesh062.get(), whiteMatte.get());
	scene.addEntity(mesh063.get(), panelling.get());
	scene.addEntity(mesh064.get(), whiteMatte.get());
	scene.addEntity(plant1_stem1.get(), stem1Mat.get());
	scene.addEntity(plant1_stem2.get(), stem2Mat.get());
	scene.addEntity(plant1_ground.get(), ground.get());
	scene.addEntity(plant1_pot.get(), pot.get());
	scene.addEntity(plant1_leaves.get(), leaf.get());
	scene.addEntity(plant2_stem1.get(), stem1Mat.get());
	scene.addEntity(plant2_stem2.get(), stem2Mat.get());
	scene.addEntity(plant2_ground.get(), ground.get());
	scene.addEntity(plant2_pot.get(), pot.get());
	scene.addEntity(plant2_leaves.get(), leaf.get());

	Renderer renderer(&scene, camera.get(), integrator.get(), sampler.get(), settings);
	savePPM("living-room-render.ppm", renderer.render());
	std::cout << "Press enter to exit" << std::endl;
	std::cin.get();
}

