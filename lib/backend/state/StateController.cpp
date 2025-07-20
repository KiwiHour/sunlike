#include "StateController.h"
#include "utils.h"

StateController g_state;
StateController &state = g_state;

Value *StateController::findValue(const std::string &name)
{
	auto it = values.find(name);
	if (it != values.end())
		return it->second.get(); // Raw pointer

	logCritical("Couldn't find state with name '%s'", name.c_str());
	return nullptr;
}

void StateController::addValue(const std::string &name, std::unique_ptr<Value> value)
{
	values[name] = std::move(value);
}

void StateController::addValue(const std::string &name, const Getter &getter, const Setter &setter)
{
	std::unique_ptr<Value> value = std::make_unique<Value>(getter, setter);
	addValue(name, std::move(value));
}

void StateController::addValue(const std::string &name, const std::pair<Getter, Setter> &getterAndSetterPair)
{
	const auto &[getter, setter] = getterAndSetterPair;
	addValue(name, getter, setter);
}

bool StateController::set(const std::string &name, int _value)
{
	Value *value = findValue(name);

	if (value == nullptr)
	{
		logCritical("Couldn't find value with name %s while trying to set it", name.c_str());
		return false;
	}

	value->set(_value);
	logDebug("State of '%s' set to '%d'", name.c_str(), _value);

	return true;
}
int StateController::get(const std::string &name)
{
	Value *value = findValue(name);
	if (value == nullptr)
	{
		logCritical("Couldn't find value with name %s while trying to get it", name.c_str());
		return 0;
	}
	return value->get();
}
void StateController::adjust(const std::string &name, int delta)
{
	int currentValue = get(name);
	set(name, currentValue + delta);
}

void StateController::fetch()
{
	for (auto &value : values)
	{
		value.second->fetch();
	}
	logDebug("Entire state fetched");
}

void StateController::fetch(const std::string &name)
{
	values[name]->fetch();
	logDebug("State of '%s' fetched", name.c_str());
}

void StateController::flush()
{
	// Flush all modified values
	for (auto &value : values)
	{
		if (value.second->isModified())
			value.second->flush();
	}
	logDebug("Entire state flushed");
}

void StateController::flush(const std::string &name)
{
	values[name]->flush();
	logDebug("State of '%s' flushed", name.c_str());
}
