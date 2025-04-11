#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "Menu.h"

using namespace std;

Adafruit_SSD1306 *Menu::screen = nullptr;

Menu::Menu(string _title, Menu *_parent)
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
	screen->setCursor(0, 0);
	int title_x = getCenteredXCoord(title, 0);

	// Draw two times with an offset to give a "bold" look
	screen->setCursor(title_x, 0);
	screen->print(title.c_str());

	screen->setCursor(title_x + 1, 0);
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