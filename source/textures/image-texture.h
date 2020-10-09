#pragma once

class ImageTexture;

#include "textures/texture.h"
#include "images/mipmap.h"

#include <memory>

class ImageTexture : public Texture {
private:
	const MipMap* m_mipmap;

public:
	ImageTexture() = delete;
	ImageTexture(const ImageTexture& other) = delete;
	ImageTexture(const MipMap* mipmap);

	Spectrum sample(const SurfacePoint& point) const;
};

