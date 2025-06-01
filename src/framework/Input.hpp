#pragma once

#include "Utils.hpp"
#include "Component.hpp"

namespace Input
{
	
	enum class MouseButton
	{
		Left,
		Middle,
		Right,
	};

	enum class Key
	{
		W,
		A,
		S,
		D,
		K,
		SPACE,
		ESCAPE,
	};

	extern std::vector<std::pair<MouseButton, Ivec2>> clickList;

	void initialize();
	void update();

	Seconds getDeltaTime();
	Seconds getRunningTime();
	
	Ivec2 getMousePosition();
	Ivec2 getWindowSize();
	float getWindowRatio();
	
	bool isMouseButtonPressed(MouseButton button);	
	bool isKeyPressed(Key key);

	bool didMouseMove();
	bool didMouseScroll();
	bool didWindowSizeChange();
	bool didWindowQuit();

	void quit();

	Vec2 getMouseMoveDelta();
	float getMouseScrollDelta();

	const std::vector<std::pair<MouseButton, Ivec2>> &getClickList();
	const std::vector<Key> &getPressList();

	void visitClickList(auto visitor)
	{
		std::erase_if(clickList, [&visitor](auto click) { return visitor(click); });
	}

};
