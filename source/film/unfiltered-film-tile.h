#pragma once

class UnfilteredFilmTile;

#include "film/film-sample.h"

#include <vector>

class UnfilteredFilmTile {
public:
	int tileStartX;
	int tileStartY;
	int tileEndX;
	int tileEndY;
	std::vector<UnfilteredFilmSample> samples;

	UnfilteredFilmTile();
	UnfilteredFilmTile(const UnfilteredFilmTile& other);
	UnfilteredFilmTile(int tileStartX, int tileStartY, int endX, int endY);
};

