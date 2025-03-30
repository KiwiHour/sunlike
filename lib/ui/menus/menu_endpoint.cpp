#ifndef ENDPOINT
#define ENDPOINT

#include "menu.h"

class MenuEndpoint : public Menu
{
public:
	bool drawn = false;
	function<bool()> func = nullptr;

	using Menu::Menu;

	void setFunction(function<bool()> _func)
	{
		func = _func;
	}

	// Will run the assigned function upon being drawn
	virtual void draw()
	{
		drawTitle();
		screen.println();

		// Check function is assigned
		if (func == nullptr)
		{
			screen.println("No function assigned :/");
			return;
		}

		bool success = func();
		string message = success ? "Success :)" : "Failed :(";

		screen.println(message.c_str());
	}

	virtual void handleInput(SwitchInput input, Menu *&currentMenu)
	{
		if (input == LEFT || input == PUSH)
			currentMenu = currentMenu->parent;
	}

	virtual void handleIdle() {}
};

#endif