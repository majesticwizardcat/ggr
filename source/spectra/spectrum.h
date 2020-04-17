#pragma once

class Spectrum;

#include "primitives/color.h"

class Spectrum {
private:
	float m_r;
	float m_g;
	float m_b;

public:
	Spectrum();
	Spectrum(const Spectrum& other);
	Spectrum(float r, float g, float b);
	Spectrum(float v);
	Spectrum(const XYZColor& xyzColor);
	Spectrum(const RGBColor& rgbColor);

	void print() const;
	void clamp();

	bool isZero() const;
	bool hasNegative() const;
	bool hasNan() const;

	float value() const;
	float luminance() const;
	float avg() const;
	float min() const;
	float max() const;

	Spectrum operator+(const Spectrum& right) const;
	Spectrum operator-(const Spectrum& right) const;
	Spectrum operator*(const Spectrum& right) const;
	Spectrum operator*(float scalar) const;
	Spectrum operator/(float scalar) const;

	Spectrum& operator+=(const Spectrum& right);
	Spectrum& operator-=(const Spectrum& right);
	Spectrum& operator*=(const Spectrum& right);
	Spectrum& operator*=(float scalar);
	Spectrum& operator/=(float scalar);

	XYZColor getXYZ() const;
	RGBColor getRGB() const;
};

