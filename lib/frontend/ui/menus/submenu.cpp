#ifndef SUBMENU
#define SUBMENU

#include "../Menu.h"

using namespace std;

class SubMenu : public Menu
{
public:
	using Menu::Menu;

	virtual void draw()
	{
		drawTitle();
		screen->print("\n");

		for (int i = 0; i < children.size(); i++)
		{
			if (i == index)
				screen->print("> ");
			else
				screen->print("  ");

			screen->println(children[i]->title.c_str());
		}
	}

	virtual InputResponse handleInput(SwitchInput input)
	{
		if (input == SwitchInput::UP)
			index--;
		if (input == SwitchInput::DOWN)
			index++;
		if (input == SwitchInput::PUSH)
			return InputResponse::GO_INTO;
		if (input == SwitchInput::LEFT)
			return InputResponse::GO_BACK;

		clampIndex(children.size() - 1);
		return InputResponse::STAY;
	}

	virtual void onIdle()
	{
		index = 0;
	}
};

#endif