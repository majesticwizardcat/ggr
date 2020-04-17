#include "spectra/spectrum.h"
#include "tools/util.h"

#include <iostream>
#include <algorithm>

Spectrum::Spectrum() : m_r(0.0f), m_g(0.0f), m_b(0.0f) { }
Spectrum::Spectrum(const Spectrum& other) : Spectrum(other.m_r, other.m_g, other.m_b) { }
Spectrum::Spectrum(float r, float g, float b) : m_r(r), m_g(g), m_b(b) { }
Spectrum::Spectrum(float v) : Spectrum(v, v, v) { }
Spectrum::Spectrum(const XYZColor& xyzColor) : Spectrum(clr::XYZToRGB(xyzColor)) { }
Spectrum::Spectrum(const RGBColor& rgbColor) : Spectrum(rgbColor.r, rgbColor.g, rgbColor.b) { }

void Spectrum::print() const {
	std::cout << "Spectrum: ";
	std::cout << "r: " << m_r << " g: " << m_g << " b: " << m_b;
}

void Spectrum::clamp() {
	m_r = util::clamp(m_r, 0.0f, 1.0f);
	m_g = util::clamp(m_g, 0.0f, 1.0f);
	m_b = util::clamp(m_b, 0.0f, 1.0f);
}

bool Spectrum::isZero() const {
	return util::equals(m_r, 0.0f)
		&& util::equals(m_g, 0.0f)
		&& util::equals(m_b, 0.0f);
}

bool Spectrum::hasNegative() const {
	return m_r < 0
		|| m_g < 0
		|| m_b < 0;
}

bool Spectrum::hasNan() const {
	return std::isinf(m_r)
		|| std::isinf(m_g)
		|| std::isinf(m_b)
		|| std::isnan(m_r)
		|| std::isnan(m_g)
		|| std::isnan(m_b);
}

float Spectrum::value() const {
	return m_r;
}

float Spectrum::luminance() const {
	return 0.2126f * m_r + 0.7152f * m_g + 0.0722f * m_b;
}

float Spectrum::avg() const {
	return (m_r + m_g + m_b) * 0.333f;
}

float Spectrum::min() const {
	return std::min(std::min(m_r, m_g), m_b);
}

float Spectrum::max() const {
	return std::max(std::max(m_r, m_g), m_b);
}

Spectrum Spectrum::operator+(const Spectrum& right) const {
	return Spectrum(m_r + right.m_r, m_g + right.m_g, m_b + right.m_b);
}

Spectrum Spectrum::operator-(const Spectrum& right) const {
	return Spectrum(m_r - right.m_r, m_g - right.m_g, m_b - right.m_b);
}

Spectrum Spectrum::operator*(const Spectrum& right) const {
	return Spectrum(m_r * right.m_r, m_g * right.m_g, m_b * right.m_b);
}

Spectrum Spectrum::operator*(float scalar) const {
	return Spectrum(m_r * scalar, m_g * scalar, m_b * scalar);
}

Spectrum Spectrum::operator/(float scalar) const {
	return Spectrum(m_r, m_g, m_b) * (1.0f / scalar);
}

Spectrum& Spectrum::operator+=(const Spectrum& right) {
	m_r += right.m_r;
	m_g += right.m_g;
	m_b += right.m_b;
	return *this;
}

Spectrum& Spectrum::operator-=(const Spectrum& right) {
	m_r -= right.m_r;
	m_g -= right.m_g;
	m_b -= right.m_b;
	return *this;
}

Spectrum& Spectrum::operator*=(const Spectrum& right) {
	m_r *= right.m_r;
	m_g *= right.m_g;
	m_b *= right.m_b;
	return *this;
}

Spectrum& Spectrum::operator*=(float scalar) {
	m_r *= scalar;
	m_g *= scalar;
	m_b *= scalar;
	return *this;
}

Spectrum& Spectrum::operator/=(float scalar) {
	m_r /= scalar;
	m_g /= scalar;
	m_b /= scalar;
	return *this;
}

XYZColor Spectrum::getXYZ() const {
	return clr::RGBToXYZ(m_r, m_g, m_b);
}

RGBColor Spectrum::getRGB() const {
	return RGBColor(m_r, m_g, m_b);
}

