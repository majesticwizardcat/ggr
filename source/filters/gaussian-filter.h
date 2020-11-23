#pragma once

class GaussianFilter;

#include "filters/filter.h"

class GaussianFilter : public Filter {
private:
	float m_alpha;

public:
	GaussianFilter();
	GaussianFilter(const GaussianFilter& other);
	GaussianFilter(float radius, float alpha);

	float evaluate1D(float centeredSamplePoint) const;
	float evaluate2D(const Point2& centeredSamplePoint) const;
	std::unique_ptr<Filter> clone() const;
};
