#include "film/film.h"
#include "spectra/spectrum.h"
#include "samplers/stohastic-sampler.h"
#include "images/image-io.h"
#include "filters/box-filter.h"

#include <iostream>

int main() {
	float resW = 150;
	float resH = 230;

	Film film(resW, resH, std::shared_ptr<Filter>(new BoxFilter(2.84f, 3.14f)));
	std::vector<UnfilteredFilmTile> tiles = film.createUnfilteredFilmTiles(16);

	Spectrum r(RGBColor(0.5f, 0.0f, 0.0f));
	Spectrum g(RGBColor(0.0f, 0.7f, 0.0f));
	Spectrum b(RGBColor(0.0f, 0.0f, 0.9f));
	Spectrum rgb = (r + g + b) / 3.0f;

	std::cout << "Adding samples to film..." << std::endl;
	for (UnfilteredFilmTile& tile : tiles) {
		for (int x = tile.tileStartX; x < tile.tileEndX; ++x) {
			for (int y = tile.tileStartY; y < tile.tileEndY; ++y) {
				Point2 filmPosition((x + 0.5f) / resW, (y + 0.5f) / resH);

				tile.samples.push_back((UnfilteredFilmSample(filmPosition, r, 1.0f)));
				tile.samples.push_back((UnfilteredFilmSample(filmPosition, g, 1.0f)));
				tile.samples.push_back((UnfilteredFilmSample(filmPosition, b, 1.0f)));
			}
		}

		film.mergeFilmTile(tile);
	}
	std::cout << "Samples added" << std::endl;

	Image image = film.getImage();
	bool correct = true;
	std::cout << "Checking result:" << std::endl;
	std::cout << "Dimensions: ";
	if (image.getWidth() != resW || image.getHeight() != resH) {
		std::cout << "FAIL";
	}
	else {
		std::cout << "PASS";
	}
	std::cout << std::endl;

	std::cout << "Pixels: ";
	for (int x = 0; x < resW; ++x) {
		if (!correct) {
			break;
		}

		for (int y = 0; y < resH; ++y) {
			if (!(image.getPixel(x, y) == rgb.getRGB())) {
				correct = false;
				std::cout << "FAIL: Value: ";
				image.getPixel(x, y).print();
				std::cout << ", Expected: ";
				rgb.getRGB().print();
				std::cout << std::endl;
				break;
			}
		}
	}
	if (correct) {
		std::cout << "PASS";
	}
	std::cout << std::endl;
}

