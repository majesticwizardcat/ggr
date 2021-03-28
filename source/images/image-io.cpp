#include "images/image-io.h"
#include "primitives/color.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string.h>

Image loadPPM(const char* location) {
	std::ifstream imageFile(location, std::ios::in);

	if (!imageFile.is_open()) {
		std::cout << "Could not open PPM file for loading: " << location << std::endl;
		return Image(1, 1);
	}

	char header[2];
	imageFile >> header;

	if (strcmp(header, "P3") != 0) {
		std::cout << location << " is not a P3 PPM file" << std::endl;
		imageFile.close();
		return Image(1, 1);
	}

	int width;
	int height;
	float maxVal;

	imageFile >> width;
	imageFile >> height;
	imageFile >> maxVal;

	Image image(width, height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float r;
			float g;
			float b;
			imageFile >> r;
			imageFile >> g;
			imageFile >> b;

			RGBColor color = RGBColor(r / maxVal, g / maxVal, b / maxVal);
			color = clr::toLinear(color);
			image.setPixel(x, y, color);
		}
	}

	imageFile.close();
	std::cout << "Loaded PPM: " << location << std::endl;
	return image;
}

void savePPM(const char* location, const Image& image) {
	std::ofstream imageFile(location, std::ios::out);

	if (!imageFile.is_open()) {
		std::cout << "Could not open PPM file for saving: " << location << std::endl;
		return;
	}

	float maxVal = 255.0f;

	imageFile << "P3" << '\n';
	imageFile << image.getWidth() << ' ' << image.getHeight() << '\n';
	imageFile << maxVal << '\n';

	for (int y = 0; y < image.getHeight(); ++y) {
		for (int x = 0; x < image.getWidth(); ++x) {
			RGBColor color = image.getPixel(x, y);
			color.clampValues();
			color = clr::tosRGB(color);
			color.r *= maxVal;
			color.g *= maxVal;
			color.b *= maxVal;
			float r = std::ceil(color.r);
			float g = std::ceil(color.g);
			float b = std::ceil(color.b);
			imageFile << r << ' ';
			imageFile << g << ' ';
			imageFile << b << ' ';
		}

		imageFile << '\n';
	}
	imageFile << std::endl;
}

