#include "film/film-sample.h"
#include "tools/util.h"

UnfilteredFilmSample::UnfilteredFilmSample() : cameraWeight(0.0f) { }
UnfilteredFilmSample::UnfilteredFilmSample(const UnfilteredFilmSample& other) :
	UnfilteredFilmSample(other.filmPosition, other.radiance, other.cameraWeight) { }
UnfilteredFilmSample::UnfilteredFilmSample(const Point2& filmPosition, const Spectrum& radiance,
	float cameraWeight) : filmPosition(filmPosition), radiance(radiance), cameraWeight(cameraWeight) { }

FilteredFilmSample::FilteredFilmSample() : FilteredFilmSample(Spectrum(), 0.0f) { }
FilteredFilmSample::FilteredFilmSample(const FilteredFilmSample& other) :
	FilteredFilmSample(other.m_unnormalizedFilteredRadiance, other.m_filterWeightSum) { }
FilteredFilmSample::FilteredFilmSample(const Spectrum& ufr, float fws) :
	m_unnormalizedFilteredRadiance(ufr), m_filterWeightSum(fws) { }

Spectrum FilteredFilmSample::getRadiance() const {
	if (util::equals(m_filterWeightSum, 0.0f)) {
		return Spectrum();
	}

	return m_unnormalizedFilteredRadiance / m_filterWeightSum;
}

FilteredFilmSample FilteredFilmSample::operator+(const FilteredFilmSample& right) const {
	return FilteredFilmSample(m_unnormalizedFilteredRadiance + right.m_unnormalizedFilteredRadiance,
		m_filterWeightSum + right.m_filterWeightSum);
}

FilteredFilmSample& FilteredFilmSample::operator+=(const FilteredFilmSample& right) {
	m_unnormalizedFilteredRadiance += right.m_unnormalizedFilteredRadiance;
	m_filterWeightSum += right.m_filterWeightSum;
	return *this;
}

