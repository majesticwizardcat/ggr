#pragma once

class BoxFilter;

#include "filters/filter.h"

class BoxFilter : public Filter {
public:
	BoxFilter();
	BoxFilter(const BoxFilter& other);
	BoxFilter(float width, float height);
	BoxFilter(float size);

	float evaluate1D(float centeredSamplePoint) const;
	float evaluate2D(const Point2& centeredSamplePoint) const;
	std::unique_ptr<Filter> clone() const;
};

