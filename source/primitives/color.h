#pragma once

class RGBColor;
class XYZColor;

class RGBColor {
public:
	float r;
	float g;
	float b;

	RGBColor();
	RGBColor(const RGBColor& other);
	RGBColor(float r, float g, float b);

	void print();
	void clampValues();
	bool operator==(const RGBColor& other) const;
};

class XYZColor {
public:
	float x;
	float y;
	float z;

	XYZColor();
	XYZColor(const XYZColor& other);
	XYZColor(float x, float y, float z);

	void print();
	void clampValues();
	bool operator==(const XYZColor& other) const;
};

namespace clr {
RGBColor toLinear(float r, float g, float b);
RGBColor toLinear(const RGBColor& rgb);
RGBColor tosRGB(float linearR, float linearG, float linearB);
RGBColor tosRGB(const RGBColor& rgb);

RGBColor XYZToRGB(float x, float y, float z);
RGBColor XYZToRGB(const XYZColor& xyz);
XYZColor RGBToXYZ(float r, float g, float b);
XYZColor RGBToXYZ(const RGBColor& rgb);
}

