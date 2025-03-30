#ifndef MENU
#define MENU

#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../switch_input.h"

using namespace std;

class Menu
{
public:
	string title;
	vector<Menu *> children = {};
	Menu *parent;
	int index = 0;

	Menu(Adafruit_SSD1306 &_screen, string _title, Menu *_parent);

	void addChildren(vector<Menu *> _children);
	void clampIndex(int upper);

	// Centered along the x-axis of the entire screen
	int getCenteredXCoord(string text, int y);

	void drawTitle();
	virtual void draw() = 0;
	virtual void handleInput(SwitchInput input, Menu *&currentMenu) = 0;
	virtual void handleIdle() = 0;

protected:
	Adafruit_SSD1306 screen;
};

#endif