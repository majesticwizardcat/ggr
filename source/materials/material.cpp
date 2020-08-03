#include "materials/material.h"

Material::Material() { }
Material::Material(const Material& other) : m_bumpMap(other.m_bumpMap) { }
Material::Material(const std::shared_ptr<BumpMap>& bumpMap) : m_bumpMap(bumpMap) { }

SurfacePoint Material::bump(const SurfacePoint& point) const {
	if (m_bumpMap) {
		return m_bumpMap->bump(point);
	}
	return point;
}

void Material::setBumpMap(const std::shared_ptr<BumpMap>& bumpMap) {
	m_bumpMap = bumpMap;
}

