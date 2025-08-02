#pragma once

#include "ui/Menu.h"
#include "config/ConfigMenu.h"
#include "tuple"

class MenuEndpoint : public Menu
{
private:
	using Args = std::vector<int>;
	using Func = std::function<bool(Args)>;
	Func func = nullptr;

public:
	using Menu::Menu;

	void setFunction(const Func &_func)
	{
		func = _func;
	}

	bool callFunction()
	{
		if (func == nullptr)
			return false;

		return true; // std::apply(func, params);
	}

	void draw();
	InputResponse handleInput(SwitchInput input);
	void onIdle() {}
};