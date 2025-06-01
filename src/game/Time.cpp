#include "Time.hpp"


void Time::start(Entity &entity)
{
	uib = &entity.getComponent<UIBehaviour>();
	setFactor(1.0f);
}

void Time::update(Entity &)
{
	if (!running) {
		return;
	}

	if (uib->speed) {
		switch (*uib->speed) {
			case 1:
				speed = Speed::Hours;
				break;
			case 2:
				speed = Speed::Days;
				break;
			case 3:
				speed = Speed::Weeks;
				break;
		}
	}
	
	gameTime += Input::getDeltaTime() * speeds[static_cast<Uint>(speed)];
	Uint seconds = static_cast<Uint>(gameTime);

	Uint daysNew = seconds / secPerDay;
	seconds -= daysNew * secPerDay;
	Uint hoursNew = seconds / secPerHour;
	seconds -= hoursNew * secPerHour;
	Uint minutesNew = seconds / secPerMinute;
	seconds -= minutesNew * secPerMinute;

	const float factorDay = (static_cast<float>(minutesNew) / 60 + static_cast<float>(hoursNew)) / 24;
	const float factorLight = sinf(factorDay * PI_F);
	setFactor(factorLight);	

	minutesNew -= minutesNew % 30;

	if (daysNew != days || hoursNew != hours || minutesNew != minutes || enabled) {
		enabled = false;
		days = daysNew;
		hours = hoursNew;
		minutes = minutesNew;
		#ifndef HEADLESS
			uib->setTime(days, hours, minutes);
		#endif
	}

	if ((hours <= 6 || hours >= 22) && poacherSpawned == false)
	{
		int ran = rand() % 100;
		if (ran == 1)
		{
			Factory::createPoacher();
			poacherSpawned = true;
		}
	}
	else if (hours > 6 && hours < 22)
	{
		poacherSpawned = false;
	}
}

void Time::setFactor(float value)
{
	factor = glm::clamp(value, 0.0f, 1.0f);
	const float strength = glm::mix(lightStrengthMin, lightStrengthMax, factor);
	auto &cameraLight = getEntity().getComponent<Light::CameraLight>();
	cameraLight.setStrength(strength);
}
