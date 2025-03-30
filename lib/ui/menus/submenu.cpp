#ifndef SUBMENU_DISPLAY
#define SUBMENU_DISPLAY

#include "menu.h"

using namespace std;

class SubMenu : public Menu
{
	using Menu::Menu;

	virtual void draw()
	{
		screen.println(title.c_str());
	}
	virtual void handleInput(SwitchInput input) {}
	virtual void handleIdle() {}
};

#endif