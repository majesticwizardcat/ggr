#pragma once

class CheckerboardImage;

#include "images/image.h"

class CheckerboardImage : public Image {
public:
	CheckerboardImage();
	CheckerboardImage(const CheckerboardImage& other);
	CheckerboardImage(int tiles);
};

