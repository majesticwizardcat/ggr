#include "film/film.h"
#include "samplers/stohastic-sampler.h"
#include "cameras/perspective-camera.h"
#include "images/image-io.h"
#include "meshes/triangle-mesh.h"
#include "tools/constants.h"
#include "filters/box-filter.h"

#include <iostream>
#include <memory>

int main() {
	int width = 400;
	int height = 300;
	Film film(width, height, std::shared_ptr<Filter>(new BoxFilter(1.5f, 1.5f)));
	PerspectiveCamera camera(std::shared_ptr<Transformation>(
	new Transformation(
	transform::lookAt(Point3(0.0f), Point3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)))),
	width, height,
	0.0f, 0.0f, PI_OVER_TWO);

	StohasticSampler sampler(width, height);
	std::shared_ptr<Vertex> v0(
		new Vertex(Point3(-1.0f, -1.0f, -2.5f), Point2(0.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f)));
	std::shared_ptr<Vertex> v1(
		new Vertex(Point3( 1.0f, -1.0f, -2.5f), Point2(1.0f, 0.0f), Normal(0.0f, 0.0f, 1.0f)));
	std::shared_ptr<Vertex> v2(
		new Vertex(Point3( 0.0f,  1.0f, -2.5f), Point2(0.5f, 1.0f), Normal(0.0f, 0.0f, 1.0f)));
	Triangle mesh(v0, v1, v2);

	std::vector<UnfilteredFilmTile> tiles = film.createUnfilteredFilmTiles(16);

	for (UnfilteredFilmTile& tile : tiles) {
		for (int x = tile.tileStartX; x < tile.tileEndX; ++x) {
			for (int y = tile.tileStartY; y < tile.tileEndY; ++y) {
				CameraSample cm = sampler.getCameraSample(Point2(x, y));
				Ray ray = camera.generateRay(cm);
				ray.createRaySpace();
				Intersection intersection;
				Spectrum rad;
				
				if (mesh.intersects(ray, &intersection)) {
					Point2 uv = intersection.intersectionPoint.uv;
					float r = uv.x;
					float g = (1.0f - r) * uv.y;
					float b = 1.0f - r - g;

					rad = Spectrum(RGBColor(r, g, b));
				}
				else {
					rad = Spectrum(RGBColor(0.05f, 0.4f, 0.05f));
				}

				UnfilteredFilmSample fm(cm.filmPosition, rad, ray.weight);
				tile.samples.push_back(fm);
			}
		}
		film.mergeFilmTile(tile);
	}

	savePPM("basic-render-test.ppm", film.getImage());
}

