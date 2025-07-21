#pragma once

#include "ui/Menu.h"
#include "state/StateController.h"
#include "ConfigController.h"

class ConfigMenu : public Menu
{
private:
	std::vector<ConfigController *> controllers = {};

public:
	using Menu::Menu;

	virtual void draw();
	void addControllers(const std::vector<ConfigController *> &_controllers);
	virtual InputResponse handleInput(SwitchInput input);
	virtual void onIdle();
	virtual void onEnter();
};