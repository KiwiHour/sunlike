#pragma once

#include <utility>
#include <functional>
#include <Arduino.h>

typedef std::pair<int, bool> StatefulValue; // Actual value and then whether or not its been modified
typedef std::function<int()> Getter;
typedef std::function<bool(int)> Setter;

class Value
{
public:
	Value(Getter _getter, Setter _setter) : getter(_getter), setter(_setter) {}

	void set(int _value);
	int get();
	void adjust(int delta);
	bool isModified();

	// Updates the internal value with the given getter
	void fetch();
	// Uses the given setter with the internal value
	bool flush();

private:
	StatefulValue value;
	Getter getter;
	Setter setter;
};