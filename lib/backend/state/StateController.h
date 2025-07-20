#pragma once

#include <unordered_map>
#include <memory>
#include "Value.h"

typedef std::function<int()> Getter;
typedef std::function<bool(int)> Setter;

class StateController
{
public:
	void addValue(const std::string &name, std::unique_ptr<Value> value);
	void addValue(const std::string &name, const Getter &getter, const Setter &setter);
	void addValue(const std::string &name, const std::pair<Getter, Setter> &getterAndSetterPair);

	bool set(const std::string &name, int _value);
	int get(const std::string &name);
	void adjust(const std::string &name, int delta);

	void fetch();
	void fetch(const std::string &name);
	void flush();
	void flush(const std::string &name);

private:
	std::unordered_map<std::string, std::unique_ptr<Value>> values;
	Value *findValue(const std::string &name);
};

extern StateController &state;