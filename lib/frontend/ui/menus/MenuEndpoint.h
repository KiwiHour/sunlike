#pragma once

#include "ui/Menu.h"

class MenuEndpoint : public Menu
{
private:
	std::function<bool()> func = nullptr;

public:
	using Menu::Menu;

	void setFunction(const std::function<bool()> &_func);

	// Will run the assigned function upon being drawn
	virtual void draw();
	virtual InputResponse handleInput(SwitchInput input);
	virtual void onIdle() {}
};