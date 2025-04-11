#ifndef BOOL_MENU_VALUE
#define BOOL_MENU_VALUE

#include "GenericMenuValue.cpp"

class BoolMenuValue : public GenericMenuValue
{
public:
	BoolMenuValue(const std::string &_stateName, string _trueString, string _falseString) : GenericMenuValue(_stateName, 0, 1)
	{
		trueString = _trueString;
		falseString = _falseString;
	}

	virtual string getFormattedValue()
	{
		if (getValue() == 0)
		{
			return falseString;
		}
		else
			return trueString;
	}

private:
	string trueString;
	string falseString;
};

#endif