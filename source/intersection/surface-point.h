#pragma once

class SurfacePoint;

#include "primitives/vector.h"
#include "primitives/point.h"
#include "meshes/vertex.h"

class SurfacePoint {
public:
	Point3 point;
	Point2 uv;
	Normal geometricNormal;
	Vector3 dpdu;
	Vector3 dpdv;
	Normal shadingNormal;
	Vector3 tangent;
	Vector3 bitangent;
	Vector2 dUVdx;
	Vector2 dUVdy;
	float surfaceArea;
	int meshID;

	SurfacePoint();
	SurfacePoint(const SurfacePoint& other);
	SurfacePoint(const Point3& point, const Point2& uv, const Normal& gNormal,
		const Vector3& dpdu, const Vector3& dpdv, const Normal& sNormal,
		const Vector3& tangent, const Vector3& bitangent, float surfaceArea, int id);
	SurfacePoint(const Vertex& v, const Normal& gNormal, const Vector3& dpdu,
		const Vector3& dpdv, const Vector3& tangent, const Vector3& bitangent,
		float surfaceArea, int id);

	void print() const;
	bool operator==(const SurfacePoint& other) const;
};

