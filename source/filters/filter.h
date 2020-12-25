#pragma once

class Filter;

const float MIN_FILTER_RADIUS = 0.5f;
const float MAX_FILTER_RADIUS = 5.0f;

#include "primitives/point.h"
#include "samplers/sample-2d.h"

#include <memory>

class Filter {
protected:
	float m_radius;

public:
	Filter() : m_radius(MIN_FILTER_RADIUS) { }
	Filter(const Filter& other) : m_radius(other.m_radius) { }
	Filter(float radius) : m_radius(std::max(std::min(radius, MAX_FILTER_RADIUS), MIN_FILTER_RADIUS)) { }

	inline float getRadius() const { return m_radius; }

	virtual float evaluate1D(float centeredSamplePoint) const = 0;
	virtual float evaluate2D(const Point2& centeredSamplePoint) const = 0;
	virtual Sample2D sample(const Sample2D& sample) const = 0;
	virtual std::unique_ptr<Filter> clone() const = 0;
};
