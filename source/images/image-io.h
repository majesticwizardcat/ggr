#pragma once

#include "images/image.h"

Image loadPPM(const char* location);
void savePPM(const char* location, const Image& image);

