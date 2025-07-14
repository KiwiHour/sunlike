#pragma once

#include "ui/Menu.h"

class MenuEndpoint : public Menu
{
public:
	using Menu::Menu;

	std::function<bool()> func = nullptr;

	void setFunction(std::function<bool()> _func);

	// Will run the assigned function upon being drawn
	virtual void draw();
	virtual InputResponse handleInput(SwitchInput input);
	virtual void onIdle() {}
};