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
	string label;
	vector<Menu *> children;
	Menu *parent;

	Menu(Adafruit_SSD1306 &_screen, string _label, Menu *_parent);

	void addChildren(vector<Menu *> _children);

	// Centered along the x-axis of the entire screen
	int getCenteredXCoord(string text, int y);

	virtual void draw() = 0;
	virtual void handleInput(SwitchInput input) = 0;
	virtual void handleIdle() = 0;

protected:
	Adafruit_SSD1306 screen;
	int index;
};

#endif