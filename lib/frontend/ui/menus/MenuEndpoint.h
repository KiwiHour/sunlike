#pragma once

#include "ui/Menu.h"
#include "config/ConfigMenu.h"
#include "tuple"

// Basically just ConfigMenu, just with a frankensteined confirm button on the end

class MenuEndpoint : public Menu
{

public:
	using Args = std::vector<int>;
	using Func = std::function<bool(Args)>;
	using Menu::Menu;

	Args getArgs();
	void setValuesControllers(const std::vector<ValuesController *> &_controllers)
	{
		internalConfigMenu.addControllers(_controllers);
		internalConfigMenu.addControllers({new ValuesController("Confirm", {})}); // Hijacking time :D
	}

	bool callFunction();
	void setFunction(const Func &_func, int _noArgs)
	{
		func = _func;
		noArgs = _noArgs;
	}

	void draw();
	InputResponse handleInput(SwitchInput input);
	virtual void onIdle() override;
	virtual void onEnter() override;

private:
	Func func = nullptr;
	bool funcExecuted = false;
	int noArgs;										// Sanity check when running callFunction
	ConfigMenu internalConfigMenu = ConfigMenu(""); // Purely to steal from handleInput and draw
};