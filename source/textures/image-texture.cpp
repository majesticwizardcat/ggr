#include "textures/image-texture.h"

ImageTexture::ImageTexture() {
	m_mipmap = std::shared_ptr<MipMap>(new MipMap());
}

ImageTexture::ImageTexture(const ImageTexture& other) : m_mipmap(other.m_mipmap) { }
ImageTexture::ImageTexture(const std::shared_ptr<MipMap>& mipmap) : m_mipmap(mipmap) { }

Spectrum ImageTexture::sample(const SurfacePoint& point) const {
	return m_mipmap->sample(point);
}

