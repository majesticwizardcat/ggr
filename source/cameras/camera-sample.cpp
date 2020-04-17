#include "cameras/camera-sample.h"

#include <iostream>

CameraSample::CameraSample() { }
CameraSample::CameraSample(const CameraSample& other) : CameraSample(other.filmPosition, other.lensPosition) { }
CameraSample::CameraSample(const Point2& filmPosition, const Point2& lensPosition) :
	filmPosition(filmPosition), lensPosition(lensPosition) { }

void CameraSample::print() {
	std::cout << "Camera Sample: film position: ";
	filmPosition.print();
	std::cout << ", lensPosition: ";
	lensPosition.print();
}

