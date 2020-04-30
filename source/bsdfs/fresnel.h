#pragma once

class Fresnel;

#include "spectra/spectrum.h"

class Fresnel {
public:
	Fresnel() { }
	virtual Spectrum evaluate(float eta, float cosTheta) const = 0;
};

class FresnelCT : public Fresnel {
public:
	FresnelCT();
	Spectrum evaluate(float eta, float cosTheta) const;
};

class FresnelSchlick : public Fresnel {
private:
	Spectrum m_R0;
	Spectrum m_oneMinusR0;

public:
	FresnelSchlick();
	FresnelSchlick(const FresnelSchlick& other);
	FresnelSchlick(const Spectrum& R0);
	Spectrum evaluate(float eta, float cosTheta) const;
};

