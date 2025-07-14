#include "BoolMenuValue.h"

std::string BoolMenuValue::getFormattedValue()
{
	if (getValue() == 0)
	{
		return falseString;
	}
	else
		return trueString;
}