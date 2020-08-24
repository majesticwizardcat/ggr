#pragma once

class Spectrum;

#include "primitives/color.h"

#include <glm/vec3.hpp>

class Spectrum : public glm::vec3 {
public:
	Spectrum();
	Spectrum(const Spectrum& other);
	Spectrum(float r, float g, float b);
	Spectrum(float v);
	Spectrum(const XYZColor& xyzColor);
	Spectrum(const RGBColor& rgbColor);

	void clamp();

	bool isZero() const;
	bool hasNegative() const;
	bool hasNan() const;

	float value() const;
	float luminance() const;
	float avg() const;
	float min() const;
	float max() const;

	XYZColor getXYZ() const;
	RGBColor getRGB() const;
};

