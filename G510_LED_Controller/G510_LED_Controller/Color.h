#pragma once
#ifndef COLOR_H_
#define COLOR_H_

//somewhere to store a HSV color
typedef struct
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
} HsvColor;

//somewhere to store an RGB color and make conversions
class Color
{
private:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
public:
	Color();

	Color(unsigned char red, unsigned char green, unsigned char blue);

	void separate(unsigned char & r, unsigned char & g, unsigned char & b);
	
};

	void HsvToRgb(HsvColor hsv, Color &c);

#endif
