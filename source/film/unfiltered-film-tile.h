#pragma once

class UnfilteredFilmTile;

#include "film/film-sample.h"

#include <vector>

class UnfilteredFilmTile {
public:
	unsigned int tileStartX;
	unsigned int tileStartY;
	unsigned int tileEndX;
	unsigned int tileEndY;
	std::vector<UnfilteredFilmSample> samples;

	UnfilteredFilmTile();
	UnfilteredFilmTile(const UnfilteredFilmTile& other);
	UnfilteredFilmTile(int tileStartX, int tileStartY, int endX, int endY);
};

