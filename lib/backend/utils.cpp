#include <string>
#include <utility>
#include <functional>
#include <Preferences.h>
#include "utils.h"

bool isPreferenceKeyTooLong(std::string key)
{
	if (key.length() > 15)
	{
		logCritical("Preference key '%s' is longer than 15 characters, which isn't allowed\n", key.c_str());
		return true;
	}
	return false;
}

int getConfig(std::string key)
{
	Preferences prefs;
	if (isPreferenceKeyTooLong(key))
	{
		logCritical("Config key '%s' too long", key);
		return -1;
	}
	prefs.begin("sunlike_config");
	int res = prefs.getInt(key.c_str());
	prefs.end();

	return res;
}
bool setConfig(std::string key, int v)
{
	Preferences prefs;
	if (isPreferenceKeyTooLong(key))
	{
		logCritical("Config key '%s' too long", key);
		return false;
	}
	prefs.begin("sunlike_config");
	bool success = prefs.putInt(key.c_str(), v);
	prefs.end();

	logDebug("'%s' set to '%s'\n", key, v);
	return success;
}

std::pair<std::function<int()>, std::function<bool(int)>> createConfigGetterAndSetter(std::string key)
{
	return {
		bind(getConfig, key),
		bind(setConfig, key, std::placeholders::_1),
	};
}

tm getCurrentTime()
{
	tm timeInfo;
	if (!getLocalTime(&timeInfo))
	{
		logCritical("Failed to get time");
		return timeInfo;
	}

	return timeInfo;
}

int getCurrentHour()
{
	return getCurrentTime().tm_hour;
}

int getCurrentMinute()
{
	return getCurrentTime().tm_min;
}
