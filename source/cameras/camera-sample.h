#pragma once

class CameraSample;

#include "primitives/point.h"

class CameraSample {
public:
	Point2 filmPosition;
	Point2 lensPosition;

	CameraSample();
	CameraSample(const CameraSample& other);
	CameraSample(const Point2& filmPosition, const Point2& lensPosition);

	void print();
};

