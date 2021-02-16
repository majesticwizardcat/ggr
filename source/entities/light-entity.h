#pragma once

class LightEntity;

#include "entities/entity.h"
#include "materials/emission-material.h"

class LightEntity : public Entity {
public:
	LightEntity() = delete;
	LightEntity(const LightEntity& other) : Entity(other) { }
	LightEntity(const Triangle* mesh, const EmissionMaterial* material, int meshID)
		: Entity(mesh, material, meshID) { }

	Spectrum emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const;
	float pdf(const Point3& surfacePoint, const SurfacePoint& lightPoint) const;
	Spectrum sample(Sampler* sampler, const Point3& surfacePoint, SurfacePoint* sampledPoint,
		Vector3* direction, float* pdf, float* lightDist) const;

	inline const LightEntity* getLight() const { return this; }
};

