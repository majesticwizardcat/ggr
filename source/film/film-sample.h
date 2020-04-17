#pragma once

class UnfilteredFilmSample;
class FilteredFilmSample;

#include "spectra/spectrum.h"
#include "primitives/point.h"

class UnfilteredFilmSample {
public:
	Point2 filmPosition;
	Spectrum radiance;
	float cameraWeight;

	UnfilteredFilmSample();
	UnfilteredFilmSample(const UnfilteredFilmSample& other);
	UnfilteredFilmSample(const Point2& filmPosition, const Spectrum& radiance,
		float cameraWeight);
};

class FilteredFilmSample {
private:
	Spectrum m_unnormalizedFilteredRadiance;
	float m_filterWeightSum;

public:
	FilteredFilmSample();
	FilteredFilmSample(const FilteredFilmSample& other);
	FilteredFilmSample(const Spectrum& ufr, float fws);

	Spectrum getRadiance() const;

	FilteredFilmSample operator+(const FilteredFilmSample& right) const;
	FilteredFilmSample& operator+=(const FilteredFilmSample& right);
};

