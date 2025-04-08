#ifndef STATE_CONTROLLER
#define STATE_CONTROLLER

#include "value.cpp"
#include <unordered_map>

typedef std::function<int()> Getter;
typedef std::function<bool(int)> Setter;

class StateController
{
public:
	void addValue(const std::string &name, Value *value);
	void addValue(const std::string &name, Getter getter, Setter setter);

	bool set(const std::string &name, int _value);
	int get(const std::string &name);
	void adjust(const std::string &name, int delta);

	void fetch();
	void flush();

private:
	std::unordered_map<std::string, Value *> values;
	Value *findValue(const std::string &name);
};

extern StateController *state;

#endif