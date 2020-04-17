#include "primitives/color.h"
#include "tools/util.h"

#include <iostream>
#include <cmath>

RGBColor::RGBColor() : r(0), g(0), b(0) { }
RGBColor::RGBColor(const RGBColor& other) : r(other.r), g(other.g), b(other.b) { }
RGBColor::RGBColor(float r, float g, float b) : r(r), g(g), b(b) { }

void RGBColor::print() {
	std::cout << "r: " << r << " g: " << g << " b: " << b;
}

void RGBColor::clampValues() {
	r = util::clamp(r, 0.0f, 1.0f);
	g = util::clamp(g, 0.0f, 1.0f);
	b = util::clamp(b, 0.0f, 1.0f);
}

bool RGBColor::operator==(const RGBColor& other) const {
	return util::equals(r, other.r)
		&& util::equals(g, other.g)
		&& util::equals(b, other.b);
}

XYZColor::XYZColor() : x(0), y(0), z(0) { }
XYZColor::XYZColor(const XYZColor& other) : x(other.x), y(other.y), z(other.z) { }
XYZColor::XYZColor(float x, float y, float z) : x(x), y(y), z(z) { }

void XYZColor::print() {
	std::cout << "x: " << x << " y: " << y << " z: " << z;
}

void XYZColor::clampValues() {
	x = util::clamp(x, 0.0f, 1.0f);
	y = util::clamp(y, 0.0f, 1.0f);
	z = util::clamp(z, 0.0f, 1.0f);
}

bool XYZColor::operator==(const XYZColor& other) const {
	return util::equals(x, other.x)
		&& util::equals(y, other.y)
		&& util::equals(z, other.z);
}

RGBColor clr::XYZToRGB(float x, float y, float z) {
	RGBColor rgb;
	rgb.r =  3.2406f * x - 1.5372f * y - 0.4986f * z;
	rgb.g = -0.9689f * x + 1.8758f * y + 0.0415f * z;
	rgb.b =  0.0557f * x - 0.2040f * y + 1.0570f * z;
	return rgb;
}

RGBColor clr::XYZToRGB(const XYZColor& xyz) {
	return XYZToRGB(xyz.x, xyz.y, xyz.z);
}

XYZColor clr::RGBToXYZ(float r, float g, float b) {
	XYZColor xyz;
	xyz.x = 0.4124f * r + 0.3576f * g + 0.1805f * b;
	xyz.y = 0.2126f * r + 0.7152f * g + 0.0722f * b;
	xyz.z = 0.0193f * r + 0.1192f * g + 0.9505f * b;
	return xyz;
}

XYZColor clr::RGBToXYZ(const RGBColor& rgb) {
	return RGBToXYZ(rgb.r, rgb.g, rgb.b);
}

RGBColor clr::toLinear(float r, float g, float b) {
	float linearR = r <= 0.04045f ? r / 12.92f : std::pow((r + 0.055f) / 1.055f, 2.4f);
	float linearG = g <= 0.04045f ? g / 12.92f : std::pow((g + 0.055f) / 1.055f, 2.4f);
	float linearB = b <= 0.04045f ? b / 12.92f : std::pow((b + 0.055f) / 1.055f, 2.4f);
	return RGBColor(linearR, linearG, linearB);
}

RGBColor clr::toLinear(const RGBColor& rgb) {
	return toLinear(rgb.r, rgb.g, rgb.b);
}

RGBColor clr::tosRGB(float linearR, float linearG, float linearB) {
	RGBColor rgb;
	rgb.r = linearR <= 0.0031f ? linearR * 12.92f : 1.055f * std::pow(linearR, 0.4166f) - 0.055f;
	rgb.g = linearG <= 0.0031f ? linearG * 12.92f : 1.055f * std::pow(linearG, 0.4166f) - 0.055f;
	rgb.b = linearB <= 0.0031f ? linearB * 12.92f : 1.055f * std::pow(linearB, 0.4166f) - 0.055f;
	return rgb;
}

RGBColor clr::tosRGB(const RGBColor& rgb) {
	return tosRGB(rgb.r, rgb.g, rgb.b);
}

