#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "../switch_input.h"
#include "menu.h"

using namespace std;

Menu::Menu(Adafruit_SSD1306 &_screen, string _label, Menu *_parent = nullptr)
{
	screen = _screen;
	label = _label;
	parent = _parent;
}

void Menu::addChildren(vector<Menu *> _children)
{
	children.insert(children.end(), _children.begin(), _children.end());
}

int Menu::getCenteredXCoord(string text, int y0)
{
	int16_t x1, y1;
	uint16_t textWidth, textHeight;
	screen.getTextBounds(text.c_str(), screen.width(), y0, &x1, &y1, &textWidth, &textHeight);
	return ((screen.width() - textWidth) / 2) - x1;
}
