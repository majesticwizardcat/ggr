#pragma once

class BumpMap;

#include "intersection/surface-point.h"

class BumpMap {
public:
	virtual void bump(SurfacePoint* point) const = 0;
};

