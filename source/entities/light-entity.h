#pragma once

class LightEntity;

#include "entities/entity.h"
#include "materials/emission-material.h"
#include "lighting/light-sample.h"

class LightEntity : public Entity {
public:
	LightEntity() = delete;
	LightEntity(const LightEntity& other) : Entity(other) { }
	LightEntity(const Triangle* mesh, const EmissionMaterial* material, int meshID)
		: Entity(mesh, material, meshID) { }

	Spectrum emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const;
	float pdf(const Point3& surfacePoint, const SurfacePoint& lightPoint) const;
	LightSample sample(Sampler* sampler, const Point3& surfacePoint) const;

	inline const LightEntity* getLight() const { return this; }
};

