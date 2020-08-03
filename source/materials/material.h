#pragma once

class Material;

#include "bsdfs/bsdf.h"
#include "intersection/surface-point.h"
#include "textures/bump-map.h"

#include <memory>

class Material {
private:
	std::shared_ptr<BumpMap> m_bumpMap;

public:
	Material();
	Material(const Material& other);
	Material(const std::shared_ptr<BumpMap>& bumpMap);

	SurfacePoint bump(const SurfacePoint& point) const;
	void setBumpMap(const std::shared_ptr<BumpMap>& bumpMap);

	virtual BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const = 0;
};

