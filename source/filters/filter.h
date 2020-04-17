#pragma once

class Filter;

const float MIN_FILTER_SIZE = 0.5f;

#include "primitives/point.h"

#include <memory>

class Filter {
protected:
	float m_width;
	float m_height;

public:
	Filter();
	Filter(const Filter& other);
	Filter(float width, float height);
	Filter(float size);

	float getWidth() const;
	float getHeight() const;

	virtual float evaluate1D(float centeredSamplePoint) const = 0;
	virtual float evaluate2D(const Point2& centeredSamplePoint) const = 0;
	virtual std::unique_ptr<Filter> clone() const = 0;
};

