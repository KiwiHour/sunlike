#pragma once

#include <memory>
#include "ui/Menu.h"
#include "state/StateController.h"
#include "GenericMenuValue.h"

class ConfigController
{
private:
	int holdCount = 0; // Amount of instances of being held down

public:
	std::string name;
	std::vector<GenericMenuValue *> values = {};

	int index = -1;

	ConfigController(const std::string &_name, const std::vector<GenericMenuValue *> &_values)
		: name(_name), values(_values) {}

	void handleInput(SwitchInput input);
	void clampIndex();
};