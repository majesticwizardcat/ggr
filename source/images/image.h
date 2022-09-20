#pragma once

class Image;

#include "primitives/color.h"

#include <memory>

class Image {
private:
	std::unique_ptr<RGBColor[]> m_pixels;
	int m_resolutionWidth;
	int m_resolutionHeight;

	int index(int x, int y) const;
public:
	Image() = delete;
	Image(const Image& other) = delete;
	Image(Image&& other) : 
		m_pixels(std::move(other.m_pixels)),
		m_resolutionWidth(std::move(other.m_resolutionWidth)),
		m_resolutionHeight(std::move(other.m_resolutionHeight)) { }

	Image(int resolutionWidth, int resolutionHeight);

	void setPixel(int x, int y, const RGBColor& color);

	bool operator==(const Image& other) const;

	int getWidth() const;
	int getHeight() const;

	RGBColor getPixel(int x, int y) const;
};

