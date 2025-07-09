#include "StateController.h"

StateController *state = new StateController();

Value *StateController::findValue(const std::string &name)
{
	auto it = values.find(name);
	if (it != values.end())
		return it->second;

	Serial.printf("Couldn't find state with name '%s'\n", name.c_str());
	return nullptr;
}

void StateController::addValue(const std::string &name, Value *value, bool fetchNow)
{
	values[name] = value;

	if (fetchNow)
		fetch(name);
}

void StateController::addValue(const std::string &name, Getter getter, Setter setter, bool fetchNow)
{
	Value *value = new Value(getter, setter);
	addValue(name, value, fetchNow);
}

void StateController::addValue(const std::string &name, std::pair<Getter, Setter> getterAndSetterPair, bool fetchNow)
{
	Getter getter = getterAndSetterPair.first;
	Setter setter = getterAndSetterPair.second;
	addValue(name, getter, setter, fetchNow);
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

void StateController::fetch(const std::string &name)
{
	values[name]->fetch();
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

void StateController::flush(const std::string &name)
{
	values[name]->flush();
}
