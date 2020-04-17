#include "core/render-settings.h"

RenderSettings::RenderSettings() : resolutionWidth(0), resolutionHeight(0),
	tileSize(0), samples(0), threads(0) { }
RenderSettings::RenderSettings(const RenderSettings& other) :
	RenderSettings(other.resolutionWidth, other.resolutionHeight, other.tileSize,
	other.samples, other.threads, other.filter) { }
RenderSettings::RenderSettings(int resW, int resH, int tileSize, int samples, int threads,
	const std::shared_ptr<Filter>& filter) :
	resolutionWidth(resW), resolutionHeight(resH), tileSize(tileSize), samples(samples),
	threads(threads), filter(filter) { }

