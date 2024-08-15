#include "filters/gaussian-filter.h"
#include "tools/constants.h"
#include "tools/util.h"

#include <cmath>

GaussianFilter::GaussianFilter() : Filter(), m_alpha(1.0f) { }
GaussianFilter::GaussianFilter(const GaussianFilter& other) :
	Filter(other), m_alpha(other.m_alpha) { }

GaussianFilter::GaussianFilter(float radius) : Filter(radius), m_alpha(0.5f / radius) { }

float GaussianFilter::evaluate1D(float centeredSamplePoint) const {
	return std::exp(-m_alpha * centeredSamplePoint * centeredSamplePoint);
}

float GaussianFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return evaluate1D(centeredSamplePoint.x) * evaluate1D(centeredSamplePoint.y);
}

Sample2D GaussianFilter::sample(const Sample2D& sample) const {
	float ln2 = 0.5f * std::sqrt(-2.0f * std::log(std::max(sample.x, 0.001f)));
	float cos = std::cos(TWO_PI * sample.y);
	float sin = std::sin(TWO_PI * sample.y);
	return Sample2D(util::clamp(ln2 * cos, -1.0f, 1.0f) * m_radius + 0.5f,
		util::clamp(ln2 * sin, -1.0f, 1.0f) * m_radius + 0.5f);
}

std::unique_ptr<Filter> GaussianFilter::clone() const {
	return std::make_unique<GaussianFilter>(*this);
}
