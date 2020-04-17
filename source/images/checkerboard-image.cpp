#include "images/checkerboard-image.h"

CheckerboardImage::CheckerboardImage() : CheckerboardImage(8) { }
CheckerboardImage::CheckerboardImage(const CheckerboardImage& other) : Image(other) { }
CheckerboardImage::CheckerboardImage(int tiles) : Image(tiles * 32, tiles * 32) {
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
			if (x == 32) {
				black = !black;
				x = 0;
			}
		}
		y++;
		if (y == 32) {
			black = !black;
			y = 0;
		}
	}
}

