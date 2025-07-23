#pragma once

#include <unordered_map>
#include <memory>
#include "Value.h"

namespace StateName
{
	namespace Bulb
	{
		inline const std::string PowerState = "bulb_power_state";
		inline const std::string Brightness = "bulb_brightness";
		inline const std::string ColorTemperature = "bulb_color_temperature";
		inline const std::string Hue = "bulb_hue";
		inline const std::string Saturation = "bulb_saturation";
	}

	namespace Sunrise
	{
		inline const std::string StartHour = "sunrise_start_hour";
		inline const std::string StartMinute = "sunrise_start_minute";
		inline const std::string DurationHour = "sunrise_duration_hour";
		inline const std::string DurationMinute = "sunrise_duration_minute";
	}

	namespace Sunset
	{
		inline const std::string StartHour = "sunset_start_hour";
		inline const std::string StartMinute = "sunset_start_minute";
		inline const std::string DurationHour = "sunset_duration_hour";
		inline const std::string DurationMinute = "sunset_duration_minute";
	}

	namespace Duskfall
	{
		inline const std::string StartHour = "duskfall_start_hour";
		inline const std::string StartMinute = "duskfall_start_minute";
		inline const std::string DurationHour = "duskfall_duration_hour";
		inline const std::string DurationMinute = "duskfall_duration_minute";
	}

	namespace Misc
	{
		// Changing these might require a restart
		inline const std::string IsDaylightSavingTime = "is_daylight_saving_time";
		inline const std::string IsManualOverride = "is_manual_override";
	}
}

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
	int fetchAndGet(const std::string &name);

	bool flush();
	bool flush(const std::string &name);
	bool setAndFlush(const std::string &name, int _value);

private:
	std::unordered_map<std::string, std::unique_ptr<Value>> values;
	Value *findValue(const std::string &name);
};

extern StateController &state;