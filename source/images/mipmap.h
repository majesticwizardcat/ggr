#pragma once

class Layer;
class MipMap;

#include "images/image.h"
#include "spectra/spectrum.h"
#include "intersection/surface-point.h"
#include "primitives/transformation.h"

#include <vector>
#include <memory>

class Layer {
private:
	std::unique_ptr<Spectrum[]> m_pixels;
	int m_width;
	int m_height;
	
	int index(int width, int height) const;

public:
	Layer() = delete;
	Layer(const Layer& other);
	Layer(int width, int height);

	Spectrum get(int x, int y) const;
	void set(int x, int y, const Spectrum& v);
	int getWidth() const;
	int getHeight() const;
};

class MipMap {
private:
	std::vector<Layer> m_mipmaps;
	int m_anisotropicLevel;

	Spectrum get(int level, int x, int y) const;
	Spectrum bilinear(int level, const Point2& uv) const;
	Spectrum trilinear(const Point2& uv, const Vector2& dUVdx, const Vector2& dUVdy) const;
	Spectrum ewa(int level, const Point2& uv, const Vector2& minorAxis,
		const Vector2& majorAxis) const;

public:
	MipMap();
	MipMap(const MipMap& other);
	MipMap(const Image& image);
	MipMap(const Image& image, int anisotropicLevel);

	Spectrum sample(const Point2& uv, const Vector2& dUVdx, const Vector2& dUVdy) const;
};

