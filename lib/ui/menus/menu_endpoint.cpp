#ifndef MENU_ENDPOINT
#define MENU_ENDPOINT

#include "menu.h"

using namespace std;

/* This menu item won't show a list of options, it'll either be confirmation message or control for a value like brightness or hue
 */
class MenuEndpoint : public Menu
{
	using Menu::Menu;

	virtual void draw()
	{
		screen.println("endpoint");
	}
	virtual void handleInput(SwitchInput input) {}
	virtual void handleIdle() {}
};

#endif