#pragma once

#include "ui/Menu.h"
#include "state/StateController.h"
#include "ValuesController.h"

class ConfigMenu : public Menu
{

public:
	using Menu::Menu;
	std::vector<ValuesController *> controllers = {};

	virtual void draw();
	void addControllers(const std::vector<ValuesController *> &_controllers);
	virtual InputResponse handleInput(SwitchInput input);
	virtual void onIdle();
	virtual void onEnter();
};