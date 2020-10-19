#pragma once

class Material;

#include "bsdfs/bsdf.h"
#include "intersection/surface-point.h"
#include "textures/bump-map.h"

#include <memory>

class Material {
private:
	const BumpMap* m_bumpMap;

public:
	Material() : m_bumpMap(nullptr) { }
	Material(const Material& other) : m_bumpMap(other.m_bumpMap) { }
	Material(const BumpMap* bumpMap) : m_bumpMap(bumpMap) { }

	void bump(SurfacePoint* point) const;
	void setBumpMap(const BumpMap* bumpMap);

	virtual BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const = 0;
};

