#pragma once

class Filter;

const float MIN_FILTER_RADIUS = 0.5f;

#include "primitives/point.h"

#include <memory>

class Filter {
protected:
	float m_radius;

public:
	Filter() : m_radius(MIN_FILTER_RADIUS) { }
	Filter(const Filter& other) : m_radius(other.m_radius) { }
	Filter(float radius) : m_radius(radius) { }

	inline float getRadius() const { return m_radius; }

	virtual float evaluate1D(float centeredSamplePoint) const = 0;
	virtual float evaluate2D(const Point2& centeredSamplePoint) const = 0;
	virtual std::unique_ptr<Filter> clone() const = 0;
};
