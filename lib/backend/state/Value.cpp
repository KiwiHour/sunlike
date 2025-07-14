#include "Value.h"

void Value::set(int _value)
{
	if (value.first != _value)
		value.second = true; // Mark as modified
	value.first = _value;
}

int Value::get()
{
	return value.first;
}

bool Value::isModified()
{
	return value.second;
}

void Value::fetch()
{
	value.first = getter();
	value.second = false; // Newest data, so unmodified
}

bool Value::flush()
{
	bool success = setter(value.first);
	if (success)
		value.second = false; // State now matches, so unmodified

	return success;
}