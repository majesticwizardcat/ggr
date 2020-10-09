#pragma once

class Film;

#include "filters/filter.h"
#include "film/film-sample.h"
#include "film/unfiltered-film-tile.h"
#include "images/image.h"

#include <memory>
#include <vector>

class Film {
private:
	float m_resolutionWidth;
	float m_resolutionHeight;
	const Filter* m_filter;
	std::unique_ptr<FilteredFilmSample[]> m_filmPixels;

	int index(int x, int y) const;

public:
	Film() = delete;
	Film(const Film& other) = delete;
	Film(float resolutionWidth, float resolutionHeight, const Filter* filter);

	void addUnfilteredSample(const UnfilteredFilmSample& sample);
	void mergeFilmTile(const UnfilteredFilmTile& tile);
	std::vector<UnfilteredFilmTile> createUnfilteredFilmTiles(int tileSize) const;
	Image getImage() const;
};

