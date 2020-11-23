#include "filters/gaussian-filter.h"
#include "tools/constants.h"

#include <cmath>

GaussianFilter::GaussianFilter() : Filter(), m_alpha(1.0f) { }
GaussianFilter::GaussianFilter(const GaussianFilter& other) : Filter(other), m_alpha(other.m_alpha) { }
GaussianFilter::GaussianFilter(float radius, float alpha) : Filter(radius), m_alpha(alpha) { }

float GaussianFilter::evaluate1D(float centeredSamplePoint) const {
	return std::exp(-m_alpha * centeredSamplePoint * centeredSamplePoint);
}

float GaussianFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return evaluate1D(centeredSamplePoint.x) * evaluate1D(centeredSamplePoint.y);
}

std::unique_ptr<Filter> GaussianFilter::clone() const {
	return std::make_unique<GaussianFilter>(*this);
}
