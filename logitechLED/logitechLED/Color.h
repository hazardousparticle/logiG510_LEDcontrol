#ifndef COLOR_H_
#define COLOR_H_


typedef struct
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
} HsvColor;

class Color
{
private:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
public:
	Color()
	{
		this->red = 0;
		this->green = 0;
		this->blue = 0;
	}

	Color(unsigned char red, unsigned char green, unsigned char blue)
	{
		this->red = red;
		this->green = green;
		this->blue = blue;
	}

	void separate(unsigned char & r, unsigned char & g, unsigned char & b)
	{
		r = this->red;
		g = this->green;
		b = this->blue;
	}
};

void HsvToRgb(HsvColor hsv, Color &c);

#endif