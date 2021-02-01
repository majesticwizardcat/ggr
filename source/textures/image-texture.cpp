#include "textures/image-texture.h"

ImageTexture::ImageTexture(const MipMap* mipmap) : m_mipmap(mipmap) { }

Spectrum ImageTexture::sample(const SurfacePoint& point) const {
	return m_mipmap->sample(point.uv * m_uvScale, point.dUVdx, point.dUVdy);
}
