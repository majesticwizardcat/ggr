#include "images/image.h"
#include "tools/util.h"

#include <iostream>

Image::Image(int resolutionWidth, int resolutionHeight) : m_resolutionWidth(resolutionWidth),
	m_resolutionHeight(resolutionHeight) {
	m_pixels = std::make_unique<RGBColor[]>(m_resolutionWidth * m_resolutionHeight);
}

int Image::index(int x, int y) const {
	return x * m_resolutionHeight + y;
}

void Image::setPixel(int x, int y, const RGBColor& color) {
	m_pixels[index(x, y)] = RGBColor(color);
}

bool Image::operator==(const Image& other) const {
	if (m_resolutionWidth != other.m_resolutionWidth
		|| m_resolutionHeight != other.m_resolutionHeight) {
		return false;
	}

	for (int x = 0; x < m_resolutionWidth; ++x) {
		for (int y = 0; y < m_resolutionHeight; ++y) {
			if (!(m_pixels[index(x, y)] == other.m_pixels[index(x, y)])) {
				return false;
			}
		}
	}

	return true;
}

int Image::getWidth() const {
	return m_resolutionWidth;
}

int Image::getHeight() const {
	return m_resolutionHeight;
}

RGBColor Image::getPixel(int x, int y) const {
	return m_pixels[index(x, y)];
}

