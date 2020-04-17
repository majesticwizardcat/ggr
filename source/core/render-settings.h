#pragma once

class RenderSettings;

#include "filters/filter.h"

#include <memory>

class RenderSettings {
public:
	int resolutionWidth;
	int resolutionHeight;
	int tileSize;
	int samples;
	int threads;
	std::shared_ptr<Filter> filter;

	RenderSettings();
	RenderSettings(const RenderSettings& other);
	RenderSettings(int resW, int resH, int tileSize, int samples, int threads,
		const std::shared_ptr<Filter>& filter);
};

