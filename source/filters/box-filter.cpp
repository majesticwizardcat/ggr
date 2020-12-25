#include "filters/box-filter.h"
#include "tools/util.h"

BoxFilter::BoxFilter() : Filter() { }
BoxFilter::BoxFilter(const BoxFilter& other) : Filter(other) { }
BoxFilter::BoxFilter(float radius) : Filter(radius) { }

float BoxFilter::evaluate1D(float centeredSamplePoint) const {
	return 1.0f;
}

float BoxFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return 1.0f;
}

Sample2D BoxFilter::sample(const Sample2D& sample) const {
	return Sample2D(
		sample.x * 2.0f * m_radius - m_radius + 0.5f,
		sample.y * 2.0f * m_radius - m_radius + 0.5f);
}

std::unique_ptr<Filter> BoxFilter::clone() const {
	return std::make_unique<BoxFilter>(*this);
}
