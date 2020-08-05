#include "images/checkerboard-image.h"

CheckerboardImage::CheckerboardImage() : CheckerboardImage(DEFAULT_TILES) { }
CheckerboardImage::CheckerboardImage(const CheckerboardImage& other) : Image(other) { }
CheckerboardImage::CheckerboardImage(int tiles) : Image(tiles * PIXELS_PER_TILE, tiles * PIXELS_PER_TILE) {
	bool black = true;
	int x = 0;
	int y = 0;
	for (int i = 0; i < getHeight(); ++i) {
		for (int j = 0; j < getWidth(); ++j) {
			if (black) {
				setPixel(i, j, RGBColor(0.05f, 0.05f, 0.05f));
			}
			else {
				setPixel(i, j, RGBColor(0.95f, 0.95f, 0.95f));
			}
			x++;
			if (x == PIXELS_PER_TILE) {
				black = !black;
				x = 0;
			}
		}
		y++;
		if (y == PIXELS_PER_TILE) {
			black = !black;
			y = 0;
		}
	}
}

Image CheckerboardImage::generateHeightMap() const {
	Image heightMap(getWidth(), getHeight());
	for (int x = 0; x < getWidth(); ++x) {
		for (int y = 0; y < getHeight(); ++y) {
			float h = (getPixel(x, y).r + 0.5f) * 0.5f;
			heightMap.setPixel(x, y, RGBColor(h, h, h));
		}
	}
	return heightMap;
}

