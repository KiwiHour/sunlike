#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "utils.h"
#include "Menu.h"

Adafruit_SSD1306 *Menu::screen = nullptr;

Menu::Menu(std::string _title, Menu *_parent)
{
	title = _title;
	parent = _parent;
}

void Menu::setScreen(Adafruit_SSD1306 *_screen)
{
	screen = _screen;
}

void Menu::addChildren(const std::vector<Menu *> &_children)
{
	children.insert(children.end(), _children.begin(), _children.end());
}

void Menu::drawHeader()
{
	int x, y = 0;

	// Draw two times with an offset to give a "bold" look
	// Draw the title
	screen->setCursor(x, y);
	screen->print(title.c_str());
	screen->setCursor(x + 1, y);
	screen->print(title.c_str());

	// Draw the time
	tm now = getCurrentTime();
	std::string currentHour = std::to_string(now.tm_hour);
	std::string currentMinute = std::to_string(now.tm_min);

	if (now.tm_hour <= 9)
		currentHour = "0" + currentHour;
	if (now.tm_min <= 9)
		currentMinute = "0" + currentMinute;

	std::string currentTime = currentHour + ":" + currentMinute;
	int rightJustifiedXCoord = getRightJustifiedXCoord(currentTime);

	screen->setCursor(rightJustifiedXCoord, y);
	screen->print(currentTime.c_str());
	screen->setCursor(rightJustifiedXCoord - 1, y);
	screen->print(currentTime.c_str());
}

void Menu::clampIndex(int upper)
{
	if (index < 0)
		index = upper;
	if (index > upper)
		index = 0;
}

int Menu::getCenteredXCoord(const std::string &text)
{
	int16_t x1, _1;
	uint16_t textWidth, _2;
	screen->getTextBounds(text.c_str(), screen->width(), 0, &x1, &_1, &textWidth, &_2);
	return ((screen->width() - textWidth) / 2) - x1;
}

int Menu::getRightJustifiedXCoord(const std::string &text)
{
	int16_t _1, _2;
	uint16_t textWidth, _3;
	screen->getTextBounds(text.c_str(), 0, 0, &_1, &_2, &textWidth, &_3);
	return screen->width() - textWidth;
}