#include "intersection/surface-point.h"

#include <iostream>

SurfacePoint::SurfacePoint() : surfaceArea(0.0f) { }
SurfacePoint::SurfacePoint(const SurfacePoint& other) :
	point(other.point), uv(other.uv), geometricNormal(other.geometricNormal), dpdu(other.dpdu),
	dpdv(other.dpdv), shadingNormal(other.shadingNormal), tangent(other.tangent),
	bitangent(other.bitangent), dUVdx(other.dUVdx), dUVdy(other.dUVdy),
	surfaceArea(other.surfaceArea), meshID(other.meshID) { }
SurfacePoint::SurfacePoint(const Point3& point, const Point2& uv, const Vector3& gVector3,
	const Vector3& dpdu, const Vector3& dpdv, const Vector3& sVector3,
	const Vector3& tangent, const Vector3& bitangent, float surfaceArea, int id) : point(point), uv(uv),
	geometricNormal(gVector3), dpdu(dpdu), dpdv(dpdv), shadingNormal(sVector3),
	tangent(tangent), bitangent(bitangent), surfaceArea(surfaceArea), meshID(id) { }
SurfacePoint::SurfacePoint(const Vertex& v, const Vector3& gVector3, const Vector3& dpdu,
	const Vector3& dpdv, const Vector3& tangent, const Vector3& bitangent, float surfaceArea, int id) :
	SurfacePoint(v.position, v.uv, gVector3, dpdu, dpdv, v.normal, tangent, bitangent, surfaceArea, id) { }

bool SurfacePoint::operator==(const SurfacePoint& other) const {
	return point == other.point
		&& uv == other.uv
		&& geometricNormal == other.geometricNormal
		&& dpdu == other.dpdu
		&& dpdv == other.dpdv
		&& shadingNormal == other.shadingNormal
		&& tangent == other.tangent
		&& bitangent == other.bitangent;
}

