#include "spectra/spectrum.h"
#include "tools/util.h"

#include <glm/common.hpp>

Spectrum::Spectrum() : glm::vec3() { }
Spectrum::Spectrum(const Spectrum& other) : glm::vec3(other) { }
Spectrum::Spectrum(float r, float g, float b) : glm::vec3(r, g, b) { }
Spectrum::Spectrum(float v) : glm::vec3(v) { }
Spectrum::Spectrum(const XYZColor& xyzColor) : Spectrum(clr::XYZToRGB(xyzColor)) { }
Spectrum::Spectrum(const RGBColor& rgbColor) : Spectrum(rgbColor.r, rgbColor.g, rgbColor.b) { }

void Spectrum::clamp() {
	util::clamp(r, 0.0f, 1.0f);
	util::clamp(g, 0.0f, 1.0f);
	util::clamp(b, 0.0f, 1.0f);
}

bool Spectrum::isZero() const {
	return *this == glm::vec3(0.0f);
}

float Spectrum::value() const {
	return r;
}

float Spectrum::luminance() const {
	return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

XYZColor Spectrum::getXYZ() const {
	return clr::RGBToXYZ(getRGB());
}

RGBColor Spectrum::getRGB() const {
	return RGBColor(r, g, b);
}

