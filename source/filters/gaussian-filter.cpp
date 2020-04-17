#include "filters/gaussian-filter.h"
#include "tools/constants.h"

#include <cmath>

GaussianFilter::GaussianFilter() : Filter(), m_alpha(1.0f) { }

GaussianFilter::GaussianFilter(const GaussianFilter& other) : Filter(other), m_alpha(other.m_alpha) { }

GaussianFilter::GaussianFilter(float width, float height, float alpha) : Filter(width, height), m_alpha(alpha) { }

GaussianFilter::GaussianFilter(float width, float height) : GaussianFilter(width, height, 1.0f) { }

GaussianFilter::GaussianFilter(float size) : GaussianFilter(size, size) { }

float GaussianFilter::evaluate1D(float centeredSamplePoint) const {
	return std::exp(-m_alpha * centeredSamplePoint * centeredSamplePoint);
}

float GaussianFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return evaluate1D(centeredSamplePoint.x) * evaluate1D(centeredSamplePoint.y);
}

std::unique_ptr<Filter> GaussianFilter::clone() const {
	return std::unique_ptr<Filter>(new GaussianFilter(*this));
}

