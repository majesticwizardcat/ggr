#pragma once

class BSDFSample;

#include "primitives/vector.h"
#include "spectra/spectrum.h"
#include "bsdfs/bxdf.h"

class BSDFSample {
public:
	Spectrum value;
	float pdf;
	Vector3 sampledDirection;
	bool isDeltaDist;
	BXDFType type;

	BSDFSample();
	BSDFSample(const BSDFSample& other);
	BSDFSample(const Spectrum value, float pdf, const Vector3& sampledDir, bool isDelta,
		BXDFType type);
};

