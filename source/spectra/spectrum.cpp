#include "spectra/spectrum.h"

Spectrum::Spectrum() : m_rgb(0.0f) { }
Spectrum::Spectrum(const Spectrum& other) : m_rgb(other.m_rgb) { }
Spectrum::Spectrum(const glm::vec3& v) : m_rgb(v) { }
Spectrum::Spectrum(float r, float g, float b) : m_rgb(r, g, b) { }
Spectrum::Spectrum(float v) : m_rgb(v) { }
Spectrum::Spectrum(const XYZColor& xyzColor) : Spectrum(clr::XYZToRGB(xyzColor)) { }
Spectrum::Spectrum(const RGBColor& rgbColor) : Spectrum(rgbColor.r, rgbColor.g, rgbColor.b) { }

XYZColor Spectrum::getXYZ() const {
	return clr::RGBToXYZ(getRGB());
}

RGBColor Spectrum::getRGB() const {
	return RGBColor(m_rgb.r, m_rgb.g, m_rgb.b);
}

