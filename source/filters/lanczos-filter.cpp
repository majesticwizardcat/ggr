#include "filters/lanczos-filter.h"
#include "tools/util.h"

#include <cmath>
#include <algorithm>

LanczosFilter::LanczosFilter() { }
LanczosFilter::LanczosFilter(const LanczosFilter& other) : Filter(other) { }
LanczosFilter::LanczosFilter(float width, float height) : Filter(width, height) { }

float LanczosFilter::L(float x, float alpha) const {
	if (util::equals(x, 0.0f)) {
		return 1.0f;
	}

	x = std::abs(x);
	if (x > alpha) {
		return 0.0f;
	}

	float denom = x * (x / alpha);
	if (denom == 0.0f) {
		return 1.0f;
	}

	return (std::sin(x) * std::sin(x / alpha)) / denom;
}

float LanczosFilter::evaluate1D(float centeredSamplePoint) const {
	return L(centeredSamplePoint, m_width);
}

float LanczosFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return L(centeredSamplePoint.x, m_width) * L(centeredSamplePoint.y, m_height);
}

std::unique_ptr<Filter> LanczosFilter::clone() const {
	return std::make_unique<LanczosFilter>(*this);
}

