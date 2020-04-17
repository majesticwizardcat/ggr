#pragma once

#include "primitives/point.h"
#include "samplers/sample-2d.h"
#include "spectra/spectrum.h"

namespace shading {
Point3 uniformSampleSphere(const Sample2D& sample);
Point3 fromSpherical(float phi, float theta);
Point2 diskSample(const Sample2D& sample);
Point2 toSpherical(const Vector3& v);
bool sameHemisphere(const Vector3& v0, const Vector3& v1);

Vector3 reflect(const Vector3& w, const Normal& n);
Vector3 refract(const Vector3& w, const Normal& n, float IORin, float IORout);

float cosTheta(const Vector3& w);
float cos2Theta(const Vector3& w);
float absCosTheta(const Vector3& w);
}

