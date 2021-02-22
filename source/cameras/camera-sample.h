#pragma once

class CameraSample;

#include "primitives/point.h"
#include "samplers/sample-2d.h"

class CameraSample {
public:
	Point2 filmPosition;
	Sample2D lensPosition;

	CameraSample() { }
	CameraSample(const CameraSample& other) : filmPosition(other.filmPosition),
		lensPosition(other.lensPosition) { }
	CameraSample(const Point2& filmPosition, const Sample2D& lensPosition) :
		filmPosition(filmPosition), lensPosition(lensPosition) { }
	CameraSample(CameraSample&& other) noexcept : filmPosition(std::move(other.filmPosition)),
		lensPosition(std::move(other.lensPosition)) { }

	inline CameraSample& operator=(CameraSample other) noexcept {
		std::swap(filmPosition, other.filmPosition);
		std::swap(lensPosition, other.lensPosition);
		return *this;
	}
};

