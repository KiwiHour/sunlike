#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <vector>
#include <utility>

#include "icons.h"
#include "switch-input.h"

class IconHandler
{
public:
	Icon icon;

	IconHandler(Adafruit_SSD1306 *_screen, Icon _icon, int _x, int _y)
		: icon(_icon), screen(_screen), bitmap(iconFrames.at(_icon)), x(_x), y(_y) {}

	void draw();
	std::pair<int, int> getPos();
	std::string getLabel();

private:
	int x;
	int y;
	Adafruit_SSD1306 *screen;
	std::array<byte, 128> bitmap;
};