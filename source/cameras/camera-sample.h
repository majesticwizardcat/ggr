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
};

