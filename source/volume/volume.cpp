#include "volume/volume.h"
#include "tools/constants.h"
#include "tools/util.h"

float Volume::phase(float aniso, float cosTheta) const {
	float denom = 1.0f + aniso * aniso + 2.0f * aniso * cosTheta;
	return (INV_FOUR_PI * (1.0f - aniso * aniso) / (denom * std::sqrt(denom)));
}

Spectrum Volume::phase(const Point3& p, float cosTheta) const {
	SurfacePoint sp;
	sp.point = p;
	Spectrum color = m_color->sample(sp);
	float aniso = m_aniso->sample(sp).value();
	return color * phase(aniso, cosTheta);
}

Spectrum Volume::samplePhase(const Point3& p, const Vector3& wo, Vector3* wi, Sampler* sampler) const {
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
	return color * phase(aniso, cosTheta);
}

float Volume::tr(const Point3& from, const Vector3& direction, float distance, Sampler* sampler) const {
	if (util::equals(distance, 0.0f)) {
		return 1.0f;
	}
	SurfacePoint sp;
	sp.point = from;
	float tr = 0.0f;
	int samples = (int) std::ceil(distance * 0.1f);
	for (int i = 0; i < samples || distance < 0.1f; ++i ) {
		float sampleDist = distance * sampler->getSample();
		sp.point = sp.point + direction * sampleDist;
		tr -= sampleDist * m_density->sample(sp).value();
		distance -= sampleDist;
	}
	return std::exp(tr);
}

float Volume::transmittance(const Point3& from, const Point3& to, Sampler* sampler) const {
	float distance = glm::distance(from, to);
	Vector3 direction = (to - from) / distance;
	return tr(from, direction, distance, sampler);
}

float Volume::sampleVolume(const Point3& p, const Vector3& direction, bool* sampledVolume, Point3* sampledPoint,
	Sampler* sampler) const {
	float u0 = sampler->getSample();
	if (util::equals(u0, 0.0f)) {
		*sampledVolume = false;
		return 0.0f;
	}
	float t = -std::log(u0);
	*sampledPoint  = p + direction * t;
	*sampledVolume = true;
	return tr(p, direction, t, sampler) / std::exp(-t);
}

