#pragma once

class HeightMap;

#include "textures/bump-map.h"
#include "textures/image-texture.h"
#include "images/mipmap.h"

#include <memory>

class HeightMap : public ImageTexture, public BumpMap {
public:
	HeightMap() = delete;
	HeightMap(const HeightMap& other) = delete;
	HeightMap(const MipMap* mipmap);

	void bump(SurfacePoint* point) const;
};

