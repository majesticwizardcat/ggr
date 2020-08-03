#pragma once

class BumpMap;

#include "intersection/surface-point.h"

class BumpMap {
public:
	virtual SurfacePoint bump(const SurfacePoint& point) const = 0;
};

