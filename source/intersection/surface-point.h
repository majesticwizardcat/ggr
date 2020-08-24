#pragma once

class SurfacePoint;

#include "primitives/vector.h"
#include "primitives/point.h"
#include "meshes/vertex.h"

class SurfacePoint {
public:
	Point3 point;
	Point2 uv;
	Vector3 geometricNormal;
	Vector3 dpdu;
	Vector3 dpdv;
	Vector3 shadingNormal;
	Vector3 tangent;
	Vector3 bitangent;
	Vector2 dUVdx;
	Vector2 dUVdy;
	float surfaceArea;
	int meshID;

	SurfacePoint();
	SurfacePoint(const SurfacePoint& other);
	SurfacePoint(const Point3& point, const Point2& uv, const Vector3& gNormal,
		const Vector3& dpdu, const Vector3& dpdv, const Vector3& sNormal,
		const Vector3& tangent, const Vector3& bitangent, float surfaceArea, int id);
	SurfacePoint(const Vertex& v, const Vector3& gNormal, const Vector3& dpdu,
		const Vector3& dpdv, const Vector3& tangent, const Vector3& bitangent,
		float surfaceArea, int id);

	bool operator==(const SurfacePoint& other) const;
};

