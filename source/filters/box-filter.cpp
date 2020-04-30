#include "filters/box-filter.h"
#include "tools/util.h"

BoxFilter::BoxFilter() : Filter() { }
BoxFilter::BoxFilter(const BoxFilter& other) : Filter(other) { }
BoxFilter::BoxFilter(float width, float height) : Filter(width, height) { }
BoxFilter::BoxFilter(float size) : Filter(size) { }

float BoxFilter::evaluate1D(float centeredSamplePoint) const {
	return 1.0f;
}

float BoxFilter::evaluate2D(const Point2& centeredSamplePoint) const {
	return 1.0f;
}

std::unique_ptr<Filter> BoxFilter::clone() const {
	return std::make_unique<BoxFilter>(*this);
}

