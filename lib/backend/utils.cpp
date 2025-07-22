#include <string>
#include <utility>
#include <functional>
#include <Preferences.h>
#include "utils.h"

const std::string PREFERENCE_NAME = "sunlike_config";

bool isPreferenceKey(const std::string &key)
{
	Preferences prefs;
	prefs.begin(PREFERENCE_NAME.c_str());
	bool isKey = prefs.isKey(key.c_str());
	prefs.end();

	return isKey;
}

bool isPreferenceKeyTooLong(const std::string &key)
{
	if (key.length() > 15)
	{
		logCritical("Preference key '%s' is longer than 15 characters, which isn't allowed", key.c_str());
		return true;
	}
	return false;
}

int getConfig(const std::string &key)
{
	Preferences prefs;
	if (isPreferenceKeyTooLong(key))
	{
		logCritical("Config key '%s' too long", key.c_str());
		return -1;
	}
	prefs.begin(PREFERENCE_NAME.c_str());
	int res = prefs.getInt(key.c_str());
	prefs.end();

	return res;
}
bool setConfig(const std::string &key, int v)
{
	Preferences prefs;
	if (isPreferenceKeyTooLong(key))
	{
		logCritical("Preference key '%s' too long", key.c_str());
		return false;
	}
	prefs.begin(PREFERENCE_NAME.c_str());
	bool success = prefs.putInt(key.c_str(), v);
	prefs.end();

	logDebug("Preference key '%s' set to '%d'", key.c_str(), v);
	return success;
}

std::pair<std::function<int()>, std::function<bool(int)>> createConfigGetterAndSetter(const std::string &key)
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
