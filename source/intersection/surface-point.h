#pragma once

struct SurfacePoint;

#include "primitives/vector.h"
#include "primitives/point.h"

struct SurfacePoint {
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

	SurfacePoint() : point(0.0f), uv(0.0f), geometricNormal(0.0f), dpdu(0.0f),
		dpdv(0.0f), shadingNormal(0.0f), tangent(0.0f), bitangent(0.0f), dUVdx(0.0f),
		dUVdy(0.0f), surfaceArea(0.0f), meshID(-1) { }
};

