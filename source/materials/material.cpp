#include "materials/material.h"

SurfacePoint Material::bump(const SurfacePoint& point) const {
	if (m_bumpMap) {
		return m_bumpMap->bump(point);
	}
	return point;
}

void Material::setBumpMap(const BumpMap* bumpMap) {
	m_bumpMap = bumpMap;
}

