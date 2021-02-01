#pragma once

class Texture;

#include "spectra/spectrum.h"
#include "intersection/surface-point.h"

class Texture {
protected:
	Point2 m_uvScale;
public:
	Texture() : m_uvScale(1.0f) { }
	virtual ~Texture() { }
	virtual Spectrum sample(const SurfacePoint& point) const = 0;
	void setUVScale(float x, float y) {
		m_uvScale = Point2(x, y);
	}
};

