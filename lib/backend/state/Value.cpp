#ifndef STATE
#define STATE

#include <utility>
#include <Arduino.h>
#include <functional>

typedef std::pair<int, bool> StatefulValue; // Actual value and then whether or not its been modified
typedef std::function<int()> Getter;
typedef std::function<bool(int)> Setter;

class Value
{
public:
	Value(Getter _getter, Setter _setter)
	{
		getter = _getter;
		setter = _setter;
	}

	void set(int _value)
	{
		if (value.first != _value)
			value.second = true; // Mark as modified
		value.first = _value;
	}
	int get()
	{
		return value.first;
	}
	void adjust(int delta)
	{
		set(get() + delta);
	}
	bool isModified()
	{
		return value.second;
	}

	// Updates the internal value with the given getter
	void fetch()
	{
		value.first = getter();
		value.second = false; // Newest data, so unmodified
	}
	// Uses the given setter with the internal value
	bool flush()
	{
		bool success = setter(value.first);
		if (success)
			value.second = false; // State now matches, so unmodified

		return success;
	}

private:
	StatefulValue value;
	Getter getter;
	Setter setter;
};

#endif