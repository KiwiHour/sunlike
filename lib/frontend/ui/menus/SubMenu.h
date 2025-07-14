#pragma once

#include "ui/Menu.h"

class SubMenu : public Menu
{
public:
	using Menu::Menu;

	virtual void draw();
	virtual InputResponse handleInput(SwitchInput input);
	virtual void onIdle();
};