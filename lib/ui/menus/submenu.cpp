#ifndef SUBMENU
#define SUBMENU

#include "menu.h"

using namespace std;

class SubMenu : public Menu
{
public:
	using Menu::Menu;

	virtual void draw()
	{
		drawTitle();
		screen.print("\n");

		for (int i = 0; i < children.size(); i++)
		{
			if (i == index)
				screen.print("> ");
			else
				screen.print("  ");

			screen.println(children[i]->title.c_str());
		}
	}

	virtual void handleInput(SwitchInput input, Menu *&currentMenu)
	{
		if (input == UP)
			index++;
		if (input == DOWN)
			index--;
		if (input == PUSH)
		{
			currentMenu = currentMenu->children[index];
			currentMenu->index = 0;
		}
		if (input == LEFT)
			currentMenu = currentMenu->parent;

		clampIndex(children.size() - 1);
	}

	virtual void handleIdle()
	{
		index = 0;
	}
};

#endif