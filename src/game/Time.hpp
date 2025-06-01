#pragma once

#include "Behaviour.hpp"
#include "Input.hpp"
#include "Labels.hpp"
#include "Light.hpp"
#include "UIBehaviour.hpp"
#include "Factory.hpp"

class Time : public Behaviour
{
public:

	friend class TimeIntegrationTest_TimeUpdate_ChangesTimeCorrectly_Test;
	friend class TimeIntegrationTest_SetFactor_ClampsValueAndSetsLightStrength_Test;

	Time() = default;

	enum class Speed
	{
		Hours,
		Days,
		Weeks,
	};

	inline Speed getSpeed() const { return speed; }
	inline Uint getDays() const { return days; }
	inline Uint getHours() const { return hours; }
	inline Uint getMinutes() const { return minutes; }

	inline void start() { running = true; enabled = true; }
	inline void stop() { running = false; }
	inline void reset() { gameTime = 12 * secPerHour; start(); }

private:

	static constexpr Uint secPerMinute = 60;
	static constexpr Uint secPerHour = 60 * secPerMinute;
	static constexpr Uint secPerDay = 24 * secPerHour;	

	static constexpr float speeds[3] = {15 * secPerMinute, secPerHour, secPerDay};

	UIBehaviour *uib;

	Speed speed = Speed::Hours;
	Uint days = 0, hours = 0, minutes = 0;

	bool running = false, enabled = false;
	
	float gameTime = 12 * secPerHour;

	static constexpr float lightStrengthMin = 0.075f;
	static constexpr float lightStrengthMax = 0.925f;

	float factor;

	void start(Entity &entity) override;
	void update(Entity &entity) override;

	void setFactor(float value);

	bool poacherSpawned;
};
