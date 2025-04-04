#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "../switch_input.h"
#include "menu.h"

using namespace std;

Adafruit_SSD1306 *Menu::screen = nullptr;

Menu::Menu(string _title, Menu *_parent = nullptr)
{
	title = _title;
	parent = _parent;
}

void Menu::setScreen(Adafruit_SSD1306 *_screen)
{
	screen = _screen;
}

void Menu::addChildren(vector<Menu *> _children)
{
	children.insert(children.end(), _children.begin(), _children.end());
}

void Menu::drawTitle()
{
	int title_x = getCenteredXCoord(title, 0);
	screen->setCursor(title_x, 0);
	screen->print(title.c_str());
}

void Menu::clampIndex(int upper)
{
	if (index < 0)
		index = upper;
	if (index > upper)
		index = 0;
}

int Menu::getCenteredXCoord(string text, int y0)
{
	int16_t x1, y1;
	uint16_t textWidth, textHeight;
	screen->getTextBounds(text.c_str(), screen->width(), y0, &x1, &y1, &textWidth, &textHeight);
	return ((screen->width() - textWidth) / 2) - x1;
}