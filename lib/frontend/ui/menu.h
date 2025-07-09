#ifndef MENU
#define MENU

#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../../../include/switch-input.h"

using namespace std;

enum class InputResponse
{
	GO_INTO,
	GO_BACK,
	STAY
};

class Menu
{
public:
	static Adafruit_SSD1306 *screen;
	string title;
	vector<Menu *> children = {};
	Menu *parent;
	int index = 0;

	Menu(string _title, Menu *_parent = nullptr);
	static void setScreen(Adafruit_SSD1306 *_screen);

	void addChildren(vector<Menu *> _children);
	void clampIndex(int upper);

	// Centered along the x-axis of the entire screen
	int getCenteredXCoord(string text);
	int getRightJustifiedXCoord(string text);

	void drawHeader();
	virtual void draw() = 0;
	virtual InputResponse handleInput(SwitchInput input) = 0;
	virtual void onIdle() = 0;
	virtual void onEnter() {}
};

#endif