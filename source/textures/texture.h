#pragma once

class Texture;

#include "spectra/spectrum.h"
#include "intersection/surface-point.h"

class Texture {
public:
	virtual Spectrum sample(const SurfacePoint& point) const = 0;
};

