#pragma once

class Film;
struct FilmBounds;
struct UnfilteredSample;
struct FilteredSample;

#include "filters/filter.h"
#include "images/image.h"
#include "tools/util.h"
#include "spectra/spectrum.h"

#include <memory>
#include <vector>

struct FilmBounds {
	std::pair<unsigned int, unsigned int> start;
	std::pair<unsigned int, unsigned int> end;

	FilmBounds() = delete;
	FilmBounds(const FilmBounds& other) : start(other.start), end(other.end) { }
	FilmBounds(const std::pair<unsigned int, unsigned int>& start,
		const std::pair<unsigned int, unsigned int>& end) : start(start), end(end) { }
};

struct UnfilteredSample {
	Point2 filmPosition;
	Spectrum radiance;
	float rayWeight;

	UnfilteredSample() = delete;
	UnfilteredSample(const UnfilteredSample& other) : filmPosition(other.filmPosition),
		radiance(other.radiance), rayWeight(other.rayWeight) { }
	UnfilteredSample(const Point2 pos, const Spectrum radiance, float weight) :
		filmPosition(pos), radiance(radiance), rayWeight(weight) { }
};

struct FilteredSample {
	Spectrum unnormalizedFilteredRadiance;
	float filterWeightSum;

	FilteredSample() : FilteredSample(Spectrum(0.0f), 0.0f) { }
	FilteredSample(const FilteredSample& other) :
		FilteredSample(other.unnormalizedFilteredRadiance, other.filterWeightSum) { }
	FilteredSample(const Spectrum& ufr, float fws) :
		unnormalizedFilteredRadiance(ufr), filterWeightSum(fws) { }

	inline Spectrum getRadiance() const {
		if (filterWeightSum == 0.0f) {
			return Spectrum(0.0f);
		}
		return unnormalizedFilteredRadiance / filterWeightSum;
	}
};

class Film {
private:
	float m_resolutionWidth;
	float m_resolutionHeight;
	const Filter* m_filter;
	std::unique_ptr<FilteredSample[]> m_filmPixels;

	inline int index(int x, int y) const { return x * (int) m_resolutionHeight + y; }

public:
	Film() = delete;
	Film(const Film& other) = delete;
	Film(float resolutionWidth, float resolutionHeight, const Filter* filter);

	void addUnfilteredSample(const Point2& filmPosition, const Spectrum& radiance, float rayWeight);
	void addUnfilteredSampleVector(std::vector<UnfilteredSample>* samples);
	std::vector<FilmBounds> splitToTiles(int tileSize) const;
	Image getImage() const;
};
