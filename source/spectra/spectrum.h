#pragma once

class Spectrum;

#include "primitives/color.h"

#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

const glm::vec3 TO_Y(0.2126f, 0.7252f, 0.0722f);

class Spectrum {
private:
	glm::vec3 m_rgb;

public:
	Spectrum();
	Spectrum(const Spectrum& other);
	Spectrum(const glm::vec3& v);
	Spectrum(float r, float g, float b);
	Spectrum(float v);
	Spectrum(const XYZColor& xyzColor);
	Spectrum(const RGBColor& rgbColor);

	XYZColor getXYZ() const;
	RGBColor getRGB() const;

	inline void clamp(float min, float max) {
		glm::clamp(m_rgb, min, max);
	}

	inline bool isZero() const {
		return m_rgb == glm::vec3(0.0f);
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

