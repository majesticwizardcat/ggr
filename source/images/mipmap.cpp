#include "images/mipmap.h"
#include "tools/util.h"

#include <cmath>
#include <algorithm>
#include <iostream>

int Layer::index(int x, int y) const {
	return x * m_height + y;
}

Layer::Layer() : m_width(0), m_height(0) { }
Layer::Layer(const Layer& other) : m_width(other.m_width),
	m_height(other.m_height) {
	m_pixels = std::unique_ptr<Spectrum[]>(new Spectrum[m_width * m_height]);
	for (int i = 0; i < m_width * m_height; ++i) {
		m_pixels[i] = other.m_pixels[i];
	}
}

Layer::Layer(int width, int height) : m_width(width), m_height(height) {
	m_pixels = std::unique_ptr<Spectrum[]>(new Spectrum[m_width * m_height]);
}

Spectrum Layer::get(int x, int y) const {
	return m_pixels[index(x, y)];
}

void Layer::set(int x, int y, const Spectrum& v) {
	m_pixels[index(x, y)] = v;
}

int Layer::getWidth() const {
	return m_width;
}

int Layer::getHeight() const {
	return m_height;
}

MipMap::MipMap() {
	m_mipmaps.push_back(Layer(1, 1));
}

MipMap::MipMap(const MipMap& other) : m_mipmaps(other.m_mipmaps) { }
MipMap::MipMap(const Image& image) : MipMap(image, 0) { }

MipMap::MipMap(const Image& image, int anisotropicLevel) : m_anisotropicLevel(anisotropicLevel) {
	int width = std::pow(2, (int) std::ceil(std::log2(image.getWidth())));
	int height = std::pow(2, (int) std::ceil(std::log2(image.getHeight())));
	Layer layer(width, height);

	float fw = ((float) ((float) width / (float) image.getWidth())) / 2.0f;
	float fh = ((float) ((float) height / (float) image.getHeight())) / 2.0f;

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			float u = (float) ((float) (x + 0.5f) / (float) width);
			float v = (float) ((float) (y + 0.5f) / (float) height);

			int sx = (int) std::max(0.0f, std::floor(u * image.getWidth() - fw));
			int sy = (int) std::max(0.0f, std::floor(v * image.getHeight() - fh));
			int ex = (int) std::min((float) image.getWidth(), std::ceil(u * image.getWidth() + fw));
			int ey = (int) std::min((float) image.getHeight(), std::ceil(v * image.getHeight() + fh));

			Spectrum value;
			int sampled = 0;
			for (int ix = sx; ix < ex; ++ix) {
				for (int iy = sy; iy < ey; ++iy) {
					value = value + Spectrum(image.getPixel(ix, iy));
					sampled++;
				}
			}

			value = value / sampled;
			layer.set(x, y, value);
		}
	}

	m_mipmaps.push_back(layer);

	while(width != 1 && height != 1) {
		width = std::max(1, width / 2);
		height = std::max(1, height / 2);
		Layer current(width, height);

		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				int sx = x * 2;
				int sy = y * 2;
				int ex = std::min(sx + 2, m_mipmaps.back().getWidth());
				int ey = std::min(sy + 2, m_mipmaps.back().getHeight());

				Spectrum value;
				int sampled = 0;
				for (int xx = sx; xx < ex; ++xx) {
					for (int yy = sy; yy < ey; ++yy) {
						value = value + m_mipmaps.back().get(xx, yy);
						sampled++;
					}
				}
				value = value / sampled;
				current.set(x, y, value);
			}
		}
		m_mipmaps.push_back(current);
	}
}

Spectrum MipMap::get(int level, int x, int y) const {
	x %= m_mipmaps[level].getWidth();
	y %= m_mipmaps[level].getHeight();
	if (x < 0) {
		x += m_mipmaps[level].getWidth();
	}
	if (y < 0) {
		y += m_mipmaps[level].getHeight();
	}
	return m_mipmaps[level].get(x, y);
}

Spectrum MipMap::bilinear(int level, const Point2& uv) const {
	float s = uv.x * m_mipmaps[level].getWidth();
	float t = uv.y * m_mipmaps[level].getHeight();
	int s0 = std::floor(s);
	int t0 = std::floor(t);
	s = s - s0;
	t = t - t0;
	return get(level, s0, t0) * (1.0f - s) * (1.0f - t)
		+ get(level, s0 + 1, t0) * s * (1.0f - t)
		+ get(level, s0, t0 + 1) * (1.0f - s) * t
		+ get(level, s0 + 1, t0 + 1) * s * t;
}

Spectrum MipMap::trilinear(const SurfacePoint& point) const {
	float width = std::max(point.dUVdx.length(), point.dUVdy.length());
	if (util::equals(width, 0.0f)) {
		return bilinear(0, point.uv);	
	}

	float level = m_mipmaps.size() - 1 + std::log2(width);
	if (level < 0) {
		return bilinear(0, point.uv);
	}

	if (level >= m_mipmaps.size() - 1) {
		return m_mipmaps.back().get(0, 0);
	}

	int lod = (int) std::floor(level);
	float t = level - lod;
	return bilinear(lod, point.uv) * (1.0f - t) + bilinear(lod + 1, point.uv) * t;
}

Spectrum MipMap::ewa(int level, const Point2& uv, const Vector2& minorAxis,
	const Vector2& majorAxis) const {
	float s = uv.x * m_mipmaps[level].getWidth();
	float t = uv.y * m_mipmaps[level].getHeight();
	float minx = minorAxis.x * m_mipmaps[level].getWidth();
	float miny = minorAxis.y * m_mipmaps[level].getHeight();
	float majx = majorAxis.x * m_mipmaps[level].getWidth();
	float majy = majorAxis.y * m_mipmaps[level].getHeight();

	float det = majx * miny - majy * minx;
	if (det == 0.0f) {
		return bilinear(level, uv);
	}

	float oneOverDet = 1.0f / det;
	float a = miny * oneOverDet;
	float b = -minx * oneOverDet;
	float c = -majy * oneOverDet;
	float d = majx * oneOverDet;

	int startX = (int) (s -std::max(std::abs(std::floor(minx)), std::abs(std::floor(majx))));
	int startY = (int) (t -std::max(std::abs(std::floor(miny)), std::abs(std::floor(majy))));
	int endX = (int) s + std::max(std::abs(std::ceil(minx)), std::abs(std::ceil(majx)));
	int endY = (int) t + std::max(std::abs(std::ceil(miny)), std::abs(std::ceil(majy)));

	Spectrum sum;
	float filterSum = 0.0f;
	for (int x = startX; x <= endX; ++x) {
		for (int y = startY; y <= endY; ++y) {
			float sx = x - s;
			float ty = y - t;
			float esX = sx * a + ty * b;
			float esY = sx * c + ty * d;
			esX *= esX;
			esY *= esY;

			if (std::sqrt(esX + esY) <= 1.0f) {
				float filter = std::exp(-2.0f * sx * sx) * std::exp(-2.0f * ty * ty);
				sum = sum + get(level, x, y) * filter;
				filterSum += filter;
			}
		}
	}

	if (util::equals(filterSum, 0.0f)) {
		return bilinear(level, uv);
	}

	return sum / filterSum;
}

Spectrum MipMap::sample(const SurfacePoint& point) const {
	if (m_anisotropicLevel < 2) {
		return trilinear(point);
	}

	Vector2 minorAxis;
	Vector2 majorAxis;

	if (point.dUVdx.lengthSquared() < point.dUVdy.lengthSquared()) {
		minorAxis = point.dUVdx;
		majorAxis = point.dUVdy;
	}

	else {
		minorAxis = point.dUVdy;
		majorAxis = point.dUVdx;
	}

	float minorAxisL = minorAxis.length();
	float majorAxisL = majorAxis.length();

	if (minorAxisL == 0.0f || majorAxis == 0.0f) {
		return bilinear(0, point.uv);
	}

	if (minorAxisL * m_anisotropicLevel < majorAxisL) {
		majorAxis = (majorAxis / majorAxisL) * (minorAxisL * m_anisotropicLevel);
	}

	float level = m_mipmaps.size() - 1 + std::log2(minorAxisL);

	if (level >= m_mipmaps.size() - 1) {
		return m_mipmaps.back().get(0, 0);
	}

	if (level < 0) {
		return ewa(0, point.uv, minorAxis, majorAxis);
	}

	int lod = (int) std::floor(level);
	float t = level - lod;
	return ewa(lod, point.uv, minorAxis, majorAxis) * (1.0f - t)
		+ ewa(lod + 1, point.uv, minorAxis, majorAxis) * t;
}

