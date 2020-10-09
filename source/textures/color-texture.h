#pragma once

class ColorTexture;

#include "textures/texture.h"

class ColorTexture : public Texture {
private:
	Spectrum m_color;

public:
	ColorTexture() = delete;
	ColorTexture(const ColorTexture& other);
	ColorTexture(const Spectrum& color);

	Spectrum sample(const SurfacePoint& point) const;
};

