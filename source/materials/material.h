#pragma once

class Material;

#include "shaders/shader.h"
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
	virtual ~Material() { }

	inline void bump(SurfacePoint* point) const {
		if (m_bumpMap) {
			return m_bumpMap->bump(point);
		}
	}

	inline void setBumpMap(const BumpMap* bumpMap) {
		m_bumpMap = bumpMap;
	}
	virtual std::unique_ptr<Shader>
		createShader(const SurfacePoint& point, const Vector3& wo) const = 0;
};
