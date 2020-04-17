#include "film/unfiltered-film-tile.h"

UnfilteredFilmTile::UnfilteredFilmTile() : UnfilteredFilmTile(0, 0, 0, 0) { }
UnfilteredFilmTile::UnfilteredFilmTile(const UnfilteredFilmTile& other) :
	UnfilteredFilmTile(other.tileStartX, other.tileStartY,
	other.tileEndX, other.tileEndY) {
	samples.insert(samples.end(), other.samples.begin(), other.samples.end());
}

UnfilteredFilmTile::UnfilteredFilmTile(int tileStartX, int tileStartY,
	int tileEndX, int tileEndY) : tileStartX(tileStartX), 
	tileStartY(tileStartY), tileEndX(tileEndX), tileEndY(tileEndY) { }

