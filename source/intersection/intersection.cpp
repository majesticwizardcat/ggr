#include "intersection/intersection.h"
#include "tools/util.h"

void Intersection::calculateScreenDifferentials(const Ray& ray) {
	if (!hit || !ray.isCameraRay) {
		return;
	}
	
	Vector3 dpdx;
	Vector3 dpdy;

	float NoP = -glm::dot(intersectionPoint.geometricNormal, intersectionPoint.point);
	float NoD = glm::dot(intersectionPoint.geometricNormal, ray.dxDirection);
	if (NoD != 0.0f) {
		float NoO = glm::dot(intersectionPoint.geometricNormal, ray.dxOrigin);
		dpdx = ray.dxOrigin + ray.dxDirection * (-(NoO + NoP) / NoD);
	}

	NoD = glm::dot(intersectionPoint.geometricNormal, ray.dyDirection);
	if (NoD != 0.0f) {
		float NoO = glm::dot(intersectionPoint.geometricNormal, ray.dyOrigin);
		dpdy = ray.dyOrigin + ray.dyDirection * (-(NoO + NoP) / NoD);
	}

	float xx = 0.0f;
	float yx = 0.0f;
	float xy = 0.0f;
	float yy = 0.0f;

	float dux = 0.0f;
	float duy = 0.0f;
	float dvx = 0.0f;
	float dvy = 0.0f;

	if (!util::equals(intersectionPoint.geometricNormal.z, 0.0f)) {
		xx = dpdx.x - intersectionPoint.point.x;
		yx = dpdx.y - intersectionPoint.point.y;
		xy = dpdy.x - intersectionPoint.point.x;
		yy = dpdy.y - intersectionPoint.point.y;

		dux = intersectionPoint.dpdu.x;
		duy = intersectionPoint.dpdu.y;
		dvx = intersectionPoint.dpdv.x;
		dvy = intersectionPoint.dpdv.y;
	}
	else if (!util::equals(intersectionPoint.geometricNormal.y, 0.0f)) {
		xx = dpdx.x - intersectionPoint.point.x;
		yx = dpdx.z - intersectionPoint.point.z;
		xy = dpdy.x - intersectionPoint.point.x;
		yy = dpdy.z - intersectionPoint.point.z;

		dux = intersectionPoint.dpdu.x;
		duy = intersectionPoint.dpdu.z;
		dvx = intersectionPoint.dpdv.x;
		dvy = intersectionPoint.dpdv.z;
	}
	else {
		xx = dpdx.y - intersectionPoint.point.y;
		yx = dpdx.z - intersectionPoint.point.z;
		xy = dpdy.y - intersectionPoint.point.y;
		yy = dpdy.z - intersectionPoint.point.z;

		dux = intersectionPoint.dpdu.y;
		duy = intersectionPoint.dpdu.z;
		dvx = intersectionPoint.dpdv.y;
		dvy = intersectionPoint.dpdv.z;
	}

	float det = dux * dvy - duy * dvx;
	if (det != 0.0f) {
		float oneOverDet = 1.0f / det;
		intersectionPoint.dUVdx.x = (xx * dvy - yx * dvx) * oneOverDet;
		intersectionPoint.dUVdx.y = (yx * dux - xx * duy) * oneOverDet;
		intersectionPoint.dUVdy.x = (xy * dvy - yy * dvx) * oneOverDet;
		intersectionPoint.dUVdy.y = (yy * dux - xy * duy) * oneOverDet;
	}
}
