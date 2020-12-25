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

class RenderTile {
private:
	unsigned int m_tileHeight;
	unsigned int m_tableWidth;
	const float* m_filterTable;

	inline unsigned int index(unsigned int x, unsigned int y) const {
		return x * m_tileHeight + y;
	}

	inline unsigned int filterIndex(unsigned int pixelX, unsigned int pixelY, const Point2& rasterLocation) const {
		unsigned int x = (unsigned int)std::floor(std::abs(rasterLocation.x - (float)pixelX));
		unsigned int y = (unsigned int)std::floor(std::abs(rasterLocation.y - (float)pixelY));
		return x * m_tableWidth + y;
	}

public:
	FilmBounds sampleBounds;
	std::unique_ptr<FilteredSample[]> samples;

	RenderTile() = delete;
	RenderTile(const RenderTile& other) = delete;
	RenderTile(const FilmBounds& bounds, const float* filterTable, unsigned int tableWidth) :
		sampleBounds(bounds), m_filterTable(filterTable), m_tableWidth(tableWidth) {
		m_tileHeight = sampleBounds.end.second - sampleBounds.start.second;
		samples = std::make_unique<FilteredSample[]>((sampleBounds.end.first - sampleBounds.start.first)
			* (sampleBounds.end.second - sampleBounds.start.second));
	}

	inline void addSample(unsigned int pixelX, unsigned int pixelY, const Point2& rasterLocation,
		const Spectrum& radiance, float rayWeight) {
		unsigned int indx = index(pixelX - sampleBounds.start.first, pixelY - sampleBounds.start.second);
		float filterValue = m_filterTable[filterIndex(pixelX, pixelY, rasterLocation)];
		samples[indx].unnormalizedFilteredRadiance += radiance * (rayWeight * filterValue);
		samples[indx].filterWeightSum += filterValue;
	}

	inline const FilteredSample& getPixel(unsigned int x, unsigned int y) const {
		return samples[index(x - sampleBounds.start.first, y - sampleBounds.start.second)];
	}
};

class Film {
private:
	unsigned int m_resolutionWidth;
	unsigned int m_resolutionHeight;
	unsigned int m_tableWidth;
	std::unique_ptr<float[]> m_filterTable;
	std::unique_ptr<FilteredSample[]> m_filmPixels;

	inline int index(int x, int y) const { return x * m_resolutionHeight + y; }
	void createFilterTable(const Filter* filter);

public:
	Film() = delete;
	Film(const Film& other) = delete;
	Film(unsigned int resolutionWidth, unsigned int resolutionHeight, const Filter* filter);

	std::unique_ptr<RenderTile> createTile(const FilmBounds& bounds) const;
	std::vector<FilmBounds> splitToTiles(unsigned int tileSize) const;
	void mergeRenderTile(const RenderTile* tile);
	Image getImage() const;
};
