#pragma once

class LanczosFilter;

#include "filters/filter.h"

class LanczosFilter : public Filter {
private:
	float L(float x, float alpha) const;

public:
	LanczosFilter();
	LanczosFilter(const LanczosFilter& other);
	LanczosFilter(float radius);

	float evaluate1D(float centeredSamplePoint) const;
	float evaluate2D(const Point2& centeredSamplePoint) const;
	Sample2D sample(const Sample2D& sample) const;
	std::unique_ptr<Filter> clone() const;
};
