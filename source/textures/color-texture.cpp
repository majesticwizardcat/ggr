#include "textures/color-texture.h"

ColorTexture::ColorTexture(const ColorTexture& other) : Texture(other) { }
ColorTexture::ColorTexture(const Spectrum& color) : Texture(), m_color(color) { }

Spectrum ColorTexture::sample(const SurfacePoint& point) const {
	return m_color;
}

