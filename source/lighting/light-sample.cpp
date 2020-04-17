#include "lighting/light-sample.h"

LightSample::LightSample() : pdf(0.0f) { }
LightSample::LightSample(const LightSample& other) :
	LightSample(other.emission, other.sampledPoint, other.pdf) { }
LightSample::LightSample(const Spectrum& emission, const SurfacePoint& sampledPoint, float pdf) :
	emission(emission), sampledPoint(sampledPoint), pdf(pdf) { }

