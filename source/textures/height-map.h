#pragma once

class HeightMap;

#include "textures/bump-map.h"
#include "textures/image-texture.h"
#include "images/mipmap.h"

#include <memory>

class HeightMap : public ImageTexture, public BumpMap {
public:
	HeightMap() = delete;
	HeightMap(const HeightMap& other);
	HeightMap(const std::shared_ptr<MipMap>& mipmap);

	SurfacePoint bump(const SurfacePoint& point) const;
};

