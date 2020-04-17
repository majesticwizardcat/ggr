#include "filters/filter.h"

#include <algorithm>

Filter::Filter() : m_width(MIN_FILTER_SIZE), m_height(MIN_FILTER_SIZE) { }
Filter::Filter(const Filter& other) : Filter(other.m_width, other.m_height) { }
Filter::Filter(float width, float height) : m_width(std::max(MIN_FILTER_SIZE, width)),
	m_height(std::max(MIN_FILTER_SIZE, height)) { }
Filter::Filter(float size) : Filter(size, size) { }

float Filter::getWidth() const {
	return m_width;
}

float Filter::getHeight() const {
	return m_height;
}

