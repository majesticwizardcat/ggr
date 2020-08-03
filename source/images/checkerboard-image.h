#pragma once

class CheckerboardImage;

#include "images/image.h"

const int DEFAULT_TILES = 8;
const int PIXELS_PER_TILE = 32;

class CheckerboardImage : public Image {
public:
	CheckerboardImage();
	CheckerboardImage(const CheckerboardImage& other);
	CheckerboardImage(int tiles);

	Image generateHeightMap() const;
};

