#include "intersection/surface-point.h"

#include <iostream>

SurfacePoint::SurfacePoint() : surfaceArea(0.0f) { }
SurfacePoint::SurfacePoint(const SurfacePoint& other) :
	point(other.point), uv(other.uv), geometricNormal(other.geometricNormal), dpdu(other.dpdu),
	dpdv(other.dpdv), shadingNormal(other.shadingNormal), tangent(other.tangent),
	bitangent(other.bitangent), dUVdx(other.dUVdx), dUVdy(other.dUVdy),
	surfaceArea(other.surfaceArea), meshID(other.meshID) { }
SurfacePoint::SurfacePoint(const Point3& point, const Point2& uv, const Normal& gNormal,
	const Vector3& dpdu, const Vector3& dpdv, const Normal& sNormal,
	const Vector3& tangent, const Vector3& bitangent, float surfaceArea, int id) : point(point), uv(uv),
	geometricNormal(gNormal), dpdu(dpdu), dpdv(dpdv), shadingNormal(sNormal),
	tangent(tangent), bitangent(bitangent), surfaceArea(surfaceArea), meshID(id) { }
SurfacePoint::SurfacePoint(const Vertex& v, const Normal& gNormal, const Vector3& dpdu,
	const Vector3& dpdv, const Vector3& tangent, const Vector3& bitangent, float surfaceArea, int id) :
	SurfacePoint(v.position, v.uv, gNormal, dpdu, dpdv, v.normal, tangent, bitangent, surfaceArea, id) { }

void SurfacePoint::print() const {
	std::cout << "Point: ";
	point.print();
	std::cout << "UV: ";
	uv.print();
	std::cout << "Geometric Normal: ";
	geometricNormal.print();
	std::cout << "dpdu: ";
	dpdu.print();
	std::cout << "dpdv: ";
	dpdv.print();
	std::cout << "Shading Normal: ";
	shadingNormal.print();
	std::cout << "Tangent: ";
	tangent.print();
	std::cout << "Bitangent: ";
	bitangent.print();
}

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

