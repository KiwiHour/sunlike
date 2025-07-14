#pragma once

#include <unordered_map>
#include "Value.h"

typedef std::function<int()> Getter;
typedef std::function<bool(int)> Setter;

class StateController
{
public:
	void addValue(const std::string &name, Value *value);
	void addValue(const std::string &name, Getter getter, Setter setter);
	void addValue(const std::string &name, std::pair<Getter, Setter> getterAndSetterPair);

	bool set(const std::string &name, int _value);
	int get(const std::string &name);
	void adjust(const std::string &name, int delta);

	void fetch();
	void fetch(const std::string &name);
	void flush();
	void flush(const std::string &name);

private:
	std::unordered_map<std::string, Value *> values;
	Value *findValue(const std::string &name);
};

extern StateController *state;