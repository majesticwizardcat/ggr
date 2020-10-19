#include "materials/material.h"

void Material::bump(SurfacePoint* point) const {
	if (m_bumpMap) {
		return m_bumpMap->bump(point);
	}
}

void Material::setBumpMap(const BumpMap* bumpMap) {
	m_bumpMap = bumpMap;
}

