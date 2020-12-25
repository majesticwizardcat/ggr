#include "film/film.h"
#include "filters/box-filter.h"

#include <cmath>
#include <algorithm>

Film::Film(unsigned int resolutionWidth, unsigned int resolutionHeight, const Filter* filter) :
	m_resolutionWidth(resolutionWidth), m_resolutionHeight(resolutionHeight) {
	m_filmPixels = std::make_unique<FilteredSample[]>(m_resolutionWidth * m_resolutionHeight);
	createFilterTable(filter);
}

void Film::createFilterTable(const Filter* filter) {
	unsigned int size = (unsigned int)std::ceil(filter->getRadius() + 0.5f) + 1;
	m_tableWidth = size;
	m_filterTable = std::make_unique<float[]>(size * size);
	for (unsigned int i = 0; i < size; ++i) {
		for (unsigned int j = 0; j < size; ++j) {
			m_filterTable[i * size + j] = filter->evaluate2D(Point2((float)i, (float)j));
		}
	}
}

std::unique_ptr<RenderTile> Film::createTile(const FilmBounds& bounds) const {
	return std::make_unique<RenderTile>(bounds, m_filterTable.get(), m_tableWidth);
}

void Film::mergeRenderTile(const RenderTile* tile) {
	for (unsigned int i = tile->sampleBounds.start.first; i < tile->sampleBounds.end.first; ++i) {
		for (unsigned int j = tile->sampleBounds.start.second; j < tile->sampleBounds.end.second; ++j) {
			const FilteredSample& sample = tile->getPixel(i, j);
			m_filmPixels[index(i, j)].unnormalizedFilteredRadiance += sample.unnormalizedFilteredRadiance;
			m_filmPixels[index(i, j)].filterWeightSum += sample.filterWeightSum;
		}
	}
}

std::vector<FilmBounds> Film::splitToTiles(unsigned int tileSize) const {
	std::vector<FilmBounds> tiles;
	unsigned int filmResW = m_resolutionWidth;
	unsigned int filmResH = m_resolutionHeight;
	unsigned int tileWidth = std::min(tileSize, filmResW);
	unsigned int tileHeight = std::min(tileSize, filmResH);

	unsigned int x, y;
	for (x = 0; x < filmResW / tileWidth; ++x) {
		for (y = 0; y < filmResH / tileHeight; ++y) {
			tiles.push_back(FilmBounds(
				std::make_pair(x * tileWidth, y * tileHeight),
				std::make_pair((x + 1) * tileWidth, (y + 1) * tileHeight)));
		}
	}

	bool shouldPad = false;

	if (filmResW % tileWidth != 0) {
		shouldPad = true;
		tiles.push_back(FilmBounds(
					std::make_pair(x * tileWidth, 0),
					std::make_pair(filmResW, y * tileHeight)));
	}

	if (filmResH % tileHeight != 0) {
		shouldPad = true;
		tiles.push_back(FilmBounds(
					std::make_pair(0, y * tileHeight),
					std::make_pair(x * tileWidth, filmResH)));
	}

	if (shouldPad) {
		tiles.push_back(FilmBounds(
					std::make_pair(x * tileWidth, y * tileHeight),
					std::make_pair(filmResW, filmResH)));
	}

	return tiles;
}

Image Film::getImage() const {
	Image image(m_resolutionWidth, m_resolutionHeight);
	for (unsigned int x = 0; x < m_resolutionWidth; ++x) {
		for (unsigned int y = 0; y < m_resolutionHeight; ++y) {
			image.setPixel(x, y, m_filmPixels[index(x, y)].getRadiance().getRGB());
		}
	}
	return image;
}
