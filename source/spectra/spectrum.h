#pragma once

class Spectrum;

#include "primitives/color.h"
#include "tools/constants.h"
#include "tools/util.h"

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

	inline bool hasNegative() const {
		return m_rgb.x < 0.0f || m_rgb.y < 0.0f || m_rgb.z < 0.0f;
	}

	inline bool isBad() const {
		return std::isnan(m_rgb.x) || std::isnan(m_rgb.y) || std::isnan(m_rgb.z)
			|| std::isinf(m_rgb.x) || std::isinf(m_rgb.y) || std::isinf(m_rgb.z)
			|| hasNegative();
	}

	inline bool checkNaNs(const char* location) const {
		if (std::isnan(m_rgb.x) || std::isnan(m_rgb.y) || std::isnan(m_rgb.z)) {
			std::cout << "Nan values on L at " << location << '\n';
			std::cin.ignore();
			return true;
		}
		if (std::isinf(m_rgb.x) || std::isinf(m_rgb.y) || std::isinf(m_rgb.z)) {
			std::cout << "Inf values on L at " << location << '\n';
			std::cin.ignore();
			return true;
		}
		return false;
	}

	inline bool isHigh() const {
		return m_rgb.r > 100.0f || m_rgb.g > 100.0f || m_rgb.b > 100.0f;
	}

	inline void print() const {
		std::cout << "Spectrum: " << m_rgb.x << " " << m_rgb.y << " " << m_rgb.z << '\n';
	}

	inline void clamp(float min, float max) {
		glm::clamp(m_rgb, min, max);
	}

	inline bool isZero() const {
		return util::equals(m_rgb.r, 0.0f)
			&& util::equals(m_rgb.g, 0.0f)
			&& util::equals(m_rgb.b, 0.0f);
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
