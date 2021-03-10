#pragma once

class Spectrum;

#include "primitives/color.h"
#include "tools/constants.h"

#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <iostream>

class Spectrum {
private:
	glm::vec3 m_rgb;

public:
	Spectrum() : m_rgb(0.0f) { }
	Spectrum(const Spectrum& other) : m_rgb(other.m_rgb) { }
	Spectrum(const glm::vec3& v) : m_rgb(v) { }
	Spectrum(float r, float g, float b) : m_rgb(r, g, b) { }
	Spectrum(float v) : m_rgb(v) { }
	Spectrum(const XYZColor& xyzColor) : Spectrum(clr::XYZToRGB(xyzColor)) { }
	Spectrum(const RGBColor& rgbColor) : Spectrum(rgbColor.r, rgbColor.g, rgbColor.b) { }

	inline XYZColor getXYZ() const {
		return clr::RGBToXYZ(getRGB());
	}

	inline RGBColor getRGB() const {
		return RGBColor(m_rgb.r, m_rgb.g, m_rgb.b);
	}

	void checkNaNs(const char* location) const {
		if (std::isnan(m_rgb.x) || std::isnan(m_rgb.y) || std::isnan(m_rgb.z)) {
			std::cout << "Nan values on L at " << location << std::endl;
			std::cin.ignore();
		}
		if (std::isinf(m_rgb.x) || std::isinf(m_rgb.y) || std::isinf(m_rgb.z)) {
			std::cout << "Inf values on L at " << location << std::endl;
			std::cin.ignore();
		}
	}

	inline void print() const {
		std::cout << "Spectrum: " << m_rgb.x << " " << m_rgb.y << " " << m_rgb.z << std::endl;
	}

	inline void clamp(float min, float max) {
		glm::clamp(m_rgb, min, max);
	}

	inline bool isZero() const {
		return m_rgb == ZERO_VECTOR;
	}

	inline Spectrum operator+(const Spectrum& right) const {
		return Spectrum(m_rgb + right.m_rgb);
	}

	inline Spectrum operator-(const Spectrum& right) const {
		return Spectrum(m_rgb - right.m_rgb);
	}

	inline Spectrum operator*(const Spectrum& right) const {
		return Spectrum(m_rgb * right.m_rgb);
	}

	inline Spectrum operator*(float scalar) const {
		return Spectrum(m_rgb * scalar);
	}

	inline Spectrum operator/(float scalar) const {
		return Spectrum(m_rgb * (1.0f / scalar));
	}

	inline Spectrum& operator=(Spectrum right) noexcept {
		std::swap(m_rgb, right.m_rgb);
		return *this;
	}

	inline Spectrum& operator+=(const Spectrum& right) {
		m_rgb += right.m_rgb;
		return *this;
	}

	inline Spectrum& operator*=(const Spectrum& right) {
		m_rgb *= right.m_rgb;
		return *this;
	}

	inline Spectrum& operator*=(float scalar) {
		m_rgb *= scalar;
		return *this;
	}

	inline Spectrum& operator/=(float scalar) {
		m_rgb *= 1.0f / scalar;
		return *this;
	}

	inline float value() const {
		return m_rgb.r;
	}

	inline float luminosity() const {
		return glm::dot(m_rgb, TO_Y);
	}
};
