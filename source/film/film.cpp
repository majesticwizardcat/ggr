#include "film/film.h"
#include "filters/box-filter.h"

#include <cmath>
#include <algorithm>

Film::Film(float resolutionWidth, float resolutionHeight, const Filter* filter) :
	m_resolutionWidth(resolutionWidth + 2.0f * std::ceil(filter->getWidth())),
	m_resolutionHeight(resolutionHeight + 2.0f * std::ceil(filter->getHeight())),
	m_filter(filter) {
	m_filmPixels = std::make_unique<FilteredSample[]>(
		(int) (m_resolutionWidth) * (int) m_resolutionHeight);
}

void Film::addUnfilteredSample(const Point2& filmPosition, const Spectrum& radiance, float rayWeight) {
	Point2 rasterPosition(filmPosition.x * m_resolutionWidth, filmPosition.y * m_resolutionHeight);
	int startX = (int) std::max(0.0f, std::floor(rasterPosition.x - m_filter->getWidth()));
	int startY = (int) std::max(0.0f, std::floor(rasterPosition.y - m_filter->getHeight()));
	int endX = (int) std::min(m_resolutionWidth, std::ceil(rasterPosition.x + m_filter->getWidth()));
	int endY = (int) std::min(m_resolutionHeight, std::ceil(rasterPosition.y + m_filter->getHeight()));
	float filterWeight = 0.0f;

	for (int x = startX; x < endX; ++x) {
		for (int y = startY; y < endY; ++y) {
			Point2 centered = Point2((rasterPosition.x - (float) x) + 0.5f,
				(rasterPosition.y - (float) y) + 0.5f);
			filterWeight = m_filter->evaluate2D(centered);
			m_filmPixels[index(x, y)].filterWeightSum += filterWeight;
			m_filmPixels[index(x, y)].unnormalizedFilteredRadiance +=
				radiance * (rayWeight * filterWeight);
		}
	}
}

void Film::addUnfilteredSampleVector(std::vector<UnfilteredSample>* samples) {
	while (!samples->empty()) {
		addUnfilteredSample(samples->back().filmPosition, samples->back().radiance, samples->back().rayWeight);
		samples->pop_back();
	}
}

std::vector<FilmBounds> Film::splitToTiles(int tileSize) const {
	std::vector<FilmBounds> tiles;
	int filmResW = m_resolutionWidth;
	int filmResH = m_resolutionHeight;
	int tileWidth = std::min(tileSize, filmResW);
	int tileHeight = std::min(tileSize, filmResH);

	int x, y;
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

