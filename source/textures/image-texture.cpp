#include "textures/image-texture.h"

ImageTexture::ImageTexture(const MipMap* mipmap) : m_mipmap(mipmap) { }

Spectrum ImageTexture::sample(const SurfacePoint& point) const {
	Point2 uv(point.uv);
	uv.y = 1.0f - uv.y;
	return m_mipmap->sample(uv * m_uvScale, point.dUVdx, point.dUVdy);
}
