#include "state_controller.h"

StateController *state = new StateController();

Value *StateController::findValue(const std::string &name)
{
	auto it = values.find(name);
	if (it != values.end())
		return it->second;

	return nullptr;
}

void StateController::addValue(const std::string &name, Value *value)
{
	values[name] = value;
}

void StateController::addValue(const std::string &name, Getter getter, Setter setter)
{
	Value *value = new Value(getter, setter);
	addValue(name, value);
}

bool StateController::set(const std::string &name, int _value)
{
	Value *value = findValue(name);

	if (value == nullptr)
		return false;

	value->set(_value);
	return true;
}
int StateController::get(const std::string &name)
{
	return findValue(name)->get();
}
void StateController::adjust(const std::string &name, int delta)
{
	findValue(name)->adjust(delta);
}

void StateController::fetch()
{
	for (auto &value : values)
	{
		value.second->fetch();
	}
}

void StateController::flush()
{
	// Flush all modified values
	for (auto &value : values)
	{
		if (value.second->isModified())
			value.second->flush();
	}
}