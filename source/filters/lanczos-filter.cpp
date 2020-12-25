#include "filters/lanczos-filter.h"
#include "tools/util.h"

#include <cmath>
#include <algorithm>

LanczosFilter::LanczosFilter() : Filter() { }
LanczosFilter::LanczosFilter(const LanczosFilter& other) : Filter(other) { }
LanczosFilter::LanczosFilter(float radius) : Filter(radius) { }

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
	return L(centeredSamplePoint, m_radius);
}

float LanczosFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return L(centeredSamplePoint.x, m_radius) * L(centeredSamplePoint.y, m_radius);
}

Sample2D LanczosFilter::sample(const Sample2D& sample) const {
	float ln2 = 0.5f * std::sqrt(-2.0f * std::log(std::max(sample.x, 0.001f)));
	float cos = std::cos(TWO_PI * sample.y);
	float sin = std::sin(TWO_PI * sample.y);
	return Sample2D(util::clamp(ln2 * cos, -1.0f, 1.0f) * m_radius + 0.5f,
		util::clamp(ln2 * sin, -1.0f, 1.0f) * m_radius + 0.5f);
}

std::unique_ptr<Filter> LanczosFilter::clone() const {
	return std::make_unique<LanczosFilter>(*this);
}
