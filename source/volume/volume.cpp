#include "volume/volume.h"
#include "tools/constants.h"
#include "tools/util.h"

#include <algorithm>

float Volume::phase(float aniso, float cosTheta) const {
	cosTheta = std::abs(cosTheta);
	float denom = 1.0f + aniso * aniso + 2.0f * aniso * cosTheta;
	return (INV_FOUR_PI * (1.0f - aniso * aniso) / (denom * std::sqrt(denom)));
}

float Volume::phase(const Point3& p, const Vector3& wo, const Vector3& wi) const {
	SurfacePoint sp;
	sp.point = p;
	float aniso = m_aniso->sample(sp).value();
	return phase(aniso, glm::dot(wo, wi));
}

float Volume::samplePhase(const Point3& p, const Vector3& wo, Vector3* wi, Sampler* sampler) const {
	Sample2D sample = sampler->getSample2D();
	SurfacePoint sp;
	sp.point = p;
	Spectrum color = m_color->sample(sp);
	float aniso = m_aniso->sample(sp).value();
	float cosTheta;
	if (util::equals(aniso, 0.0f)) {
		cosTheta = 1.0f - 2.0f * sample.x;
	}
	else {
		float rat = (1.0f - aniso * aniso) / (1.0f - aniso + 2.0f * aniso * sample.x);
		cosTheta = (1.0F + aniso * aniso - rat * rat) / (2.0f * aniso);
	}
	float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
	float phi = TWO_PI * sample.y;
	float sign = (wo.z >= 0.0) ? 1.0 : -1.0;
	float a = -1.0 / (sign + wo.z);
	float b = wo.x * wo.y * a;
	Vector3 t(1.0 + sign * wo.x * wo.x * a, sign * b, -sign * wo.x);
	Vector3 bt(b, sign + wo.y * wo.y * a, -wo.y);
	*wi = sinTheta * std::cos(phi) * t
		+ sinTheta * std::sin(phi) * bt
		+ cosTheta * wo;
	return phase(aniso, cosTheta);
}

Spectrum Volume::tr(const Point3& from, const Vector3& direction, float distance, Sampler* sampler) const {
	if (util::equals(distance, 0.0f)) {
		return 1.0f;
	}
	SurfacePoint sp;
	int samples = (int) std::ceil(distance * 0.1f);
	SamplerGen sg(sampler);
	std::vector<float> sampleValues;
	std::generate_n(sampleValues.begin(), samples, sg);
	std::sort(sampleValues.begin(), sampleValues.end());
	sampleValues.push_back(1.0f);
	float tr = 0.0f;
	Spectrum color(0.0f);
	float lastS = 0.0f;
	float lastDist = 0.0f;
	float curDist;
	for (auto s : sampleValues) {
		float curDist = distance * s;
		sp.point = from + direction * curDist;
		color += m_color->sample(sp) * (s - lastS);
		tr -= (curDist - lastDist) * m_density->sample(sp).value();
		lastDist = curDist;
		lastS = s;
	}
	return color * std::exp(tr);
}

Spectrum Volume::transmittance(const Point3& from, const Point3& to, Sampler* sampler) const {
	float distance = glm::distance(from, to);
	Vector3 direction = (to - from) / distance;
	return tr(from, direction, distance, sampler);
}

Spectrum Volume::sampleVolume(const Point3& p, const Vector3& direction, bool* sampledVolume, Point3* sampledPoint,
	Sampler* sampler) const {
	float u0 = sampler->getSample();
	if (util::equals(u0, 0.0f)) {
		*sampledVolume = false;
		return 0.0f;
	}
	float t = -std::log(u0);
	*sampledPoint  = p + direction * t;
	*sampledVolume = true;
	return tr(p, direction, t, sampler) / u0; // pdf = exp(-t) = exp(ln(u0)) = u0
}

