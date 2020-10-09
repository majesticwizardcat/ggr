#include "film/film.h"
#include "filters/box-filter.h"

#include <cmath>
#include <algorithm>

Film::Film(float resolutionWidth, float resolutionHeight, const Filter* filter) :
	m_resolutionWidth(resolutionWidth + 2.0f * std::ceil(filter->getWidth())),
	m_resolutionHeight(resolutionHeight + 2.0f * std::ceil(filter->getHeight())),
	m_filter(filter) {
	m_filmPixels = std::make_unique<FilteredFilmSample[]>(
		(int) (m_resolutionWidth) * (int) m_resolutionHeight);
}

int Film::index(int x, int y) const {
	return x * m_resolutionHeight + y;
}

void Film::addUnfilteredSample(const UnfilteredFilmSample& sample) {
	Point2 rasterPosition(sample.filmPosition.x * m_resolutionWidth, sample.filmPosition.y * m_resolutionHeight);
	int startX = (int) std::max(0.0f, std::floor(rasterPosition.x - m_filter->getWidth()));
	int startY = (int) std::max(0.0f, std::floor(rasterPosition.y - m_filter->getHeight()));
	int endX = (int) std::min(m_resolutionWidth, std::ceil(rasterPosition.x + m_filter->getWidth()));
	int endY = (int) std::min(m_resolutionHeight, std::ceil(rasterPosition.y + m_filter->getHeight()));

	for (int x = startX; x < endX; ++x) {
		for (int y = startY; y < endY; ++y) {
			Point2 centered = Point2((rasterPosition.x - (float) x) + 0.5f,
				(rasterPosition.y - (float) y) + 0.5f);
			float filterWeight = m_filter->evaluate2D(centered);
			Spectrum unormFltrRad = sample.radiance * sample.cameraWeight * filterWeight;
			m_filmPixels[index(x, y)] += FilteredFilmSample(unormFltrRad, filterWeight);
		}
	}
}

void Film::mergeFilmTile(const UnfilteredFilmTile& tile) {
	for (const UnfilteredFilmSample& sample : tile.samples) {
		addUnfilteredSample(sample);
	}
}

std::vector<UnfilteredFilmTile> Film::createUnfilteredFilmTiles(int tileSize) const {
	std::vector<UnfilteredFilmTile> tiles;
	int filmResW = m_resolutionWidth;
	int filmResH = m_resolutionHeight;
	int tileWidth = std::min(tileSize, filmResW);
	int tileHeight = std::min(tileSize, filmResH);

	int x, y;
	for (x = 0; x < filmResW / tileWidth; ++x) {
		for (y = 0; y < filmResH / tileHeight; ++y) {
			tiles.push_back(UnfilteredFilmTile(
					x * tileWidth,
					y * tileHeight, 
					(x + 1) * tileWidth,
					(y + 1) * tileHeight));
		}
	}

	bool shouldPad = false;

	if (filmResW % tileWidth != 0) {
		shouldPad = true;
		tiles.push_back(UnfilteredFilmTile(
					x * tileWidth,
					0,
					filmResW,
					y * tileHeight));
	}

	if (filmResH % tileHeight != 0) {
		shouldPad = true;
		tiles.push_back(UnfilteredFilmTile(
					0,
					y * tileHeight,
					x * tileWidth,
					filmResH));
	}

	if (shouldPad) {
		tiles.push_back(UnfilteredFilmTile(
					x * tileWidth,
					y * tileHeight,
					filmResW,
					filmResH));
	}

	return tiles;
}

Image Film::getImage() const {
	int filterW = std::ceil(m_filter->getWidth());
	int filterH = std::ceil(m_filter->getHeight());
	int startX = filterW;
	int startY = filterH;
	int endX = m_resolutionWidth - filterW;
	int endY = m_resolutionHeight - filterH;

	Image image(m_resolutionWidth - 2.0f * filterW,
		m_resolutionHeight - 2.0f * filterH);

	for (int x = startX; x < endX; ++x) {
		for (int y = startY; y < endY; ++y) {
			image.setPixel(x - filterW, y - filterH, m_filmPixels[index(x, y)].getRadiance().getRGB());
		}
	}

	return image;
}

