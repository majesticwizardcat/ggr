#include "cameras/camera-sample.h"

#include <iostream>

CameraSample::CameraSample() { }
CameraSample::CameraSample(const CameraSample& other) : CameraSample(other.filmPosition, other.lensPosition) { }
CameraSample::CameraSample(const Point2& filmPosition, const Point2& lensPosition) :
	filmPosition(filmPosition), lensPosition(lensPosition) { }

