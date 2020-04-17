#include "bsdfs/bsdf-sample.h"

BSDFSample::BSDFSample() : pdf(0.0f), isDeltaDist(false), type(BXDFType::NONE) { }
BSDFSample::BSDFSample(const BSDFSample& other) : BSDFSample(other.value, other.pdf, other.sampledDirection,
	other.isDeltaDist, other.type) { }
BSDFSample::BSDFSample(const Spectrum value, float pdf, const Vector3& sampledDir, bool isDelta,
	BXDFType type) :
	value(value), pdf(pdf), sampledDirection(sampledDir), isDeltaDist(isDelta), type(type) { }

