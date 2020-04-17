#pragma once

class ImageTexture;

#include "textures/texture.h"
#include "images/mipmap.h"

#include <memory>

class ImageTexture : public Texture {
private:
	std::shared_ptr<MipMap> m_mipmap;

public:
	ImageTexture();
	ImageTexture(const ImageTexture& other);
	ImageTexture(const std::shared_ptr<MipMap>& mipmap);

	Spectrum sample(const SurfacePoint& point) const;
};

