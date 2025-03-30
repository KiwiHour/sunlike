#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <vector>
#include "icons.h"

using namespace std;

class IconHandler
{
public:
	Icon icon;

	IconHandler(Adafruit_SSD1306 &_screen, Icon _icon, int _x, int _y)
	{
		icon = _icon;
		screen = _screen;
		bitmap = iconFrames[icon];
		x = _x;
		y = _y;
	}

	pair<int, int> getPos()
	{
		return {x, y};
	}

	void draw()
	{
		screen.drawBitmap(x, y, bitmap.data(), 32, 32, WHITE);
	}

	std::string getLabel()
	{
		switch (icon)
		{
		case FUNCTIONS:
			return "Funcs";
		case CONFIG:
			return "Config";
		case ADJUST:
			return "Adjust";
		default:
			return "Unknown";
		}
	}

private:
	int x;
	int y;
	Adafruit_SSD1306 screen;
	array<byte, 128> bitmap;
};