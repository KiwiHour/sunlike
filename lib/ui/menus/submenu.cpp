#ifndef SUBMENU
#define SUBMENU

#include "menu.h"

using namespace std;

class SubMenu : public Menu
{
	using Menu::Menu;

	virtual void draw()
	{
		drawTitle();
		screen.println(title.c_str());
	}
	virtual void handleInput(SwitchInput input) {}
	virtual void handleIdle() {}
};

#endif