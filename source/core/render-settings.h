#pragma once

struct RenderSettings;

#include "filters/filter.h"

struct RenderSettings {
	unsigned int resolutionWidth;
	unsigned int resolutionHeight;
	unsigned int tileSize;
	unsigned int samples;
	unsigned int threads;
	const Filter* filter;

	RenderSettings() = delete;
	RenderSettings(const RenderSettings& other) : resolutionWidth(other.resolutionWidth),
		resolutionHeight(other.resolutionHeight), tileSize(other.tileSize), samples(other.samples),
		threads(other.threads), filter(other.filter) { }

	RenderSettings(unsigned int resW, unsigned int resH, unsigned int tileSize,
		unsigned int samples, unsigned int threads, const Filter* filter) :
		resolutionWidth(resW), resolutionHeight(resH), tileSize(tileSize), samples(samples),
		threads(threads), filter(filter) { }
};

