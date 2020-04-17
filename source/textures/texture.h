#pragma once

class Texture;

#include "spectra/spectrum.h"
#include "intersection/surface-point.h"

class Texture {
public:
	Texture();
	Texture(const Texture& other);

	virtual Spectrum sample(const SurfacePoint& point) const = 0;
};

