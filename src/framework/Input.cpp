#include <optional>
#include <unordered_map>
#include <cstdlib>

#ifndef HEADLESS
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#endif // HEADLESS

#include "Input.hpp"

#ifndef HEADLESS
extern SDL_Window *window;
#endif // HEADLESS

namespace Input
{
#ifndef HEADLESS
	static Uint64 ticksBegin, ticksLast, ticksNow;
#endif // HEADLESS
	static Ivec2 mousePos;
	static bool mouseLeftDown, mouseMiddleDown, mouseRightDown;
	static std::unordered_map<Key, bool> keyPressed;
	static Ivec2 windowSize;
	
	static bool didMouseMoveFlag;
	static bool didMouseScrollFlag;
	static bool didWindowSizeChangeFlag;
	static bool didWindowQuitFlag;
	static Vec2 mouseMoveDelta;
	static float mouseScrollDelta;

	std::vector<std::pair<MouseButton, Ivec2>> clickList;
	std::vector<Key> pressList;

	void initialize()
	{
#ifndef HEADLESS
		ticksBegin = ticksLast = ticksNow = SDL_GetTicks64();
		const Uint32 bitmap = SDL_GetMouseState(&mousePos.x, &mousePos.y);
		mouseLeftDown = bitmap & SDL_BUTTON(1);
		mouseMiddleDown = bitmap & SDL_BUTTON(2);
		mouseRightDown = bitmap & SDL_BUTTON(3);
		SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
#endif // HEADLESS
	}

	void update()
	{
#ifndef HEADLESS
		didMouseMoveFlag = false;
		didMouseScrollFlag = false;
		didWindowSizeChangeFlag = false;
		didWindowQuitFlag = false;
		
		ticksLast = ticksNow;
		ticksNow = SDL_GetTicks64();

		mouseMoveDelta = {};
		mouseScrollDelta = {};

		clickList.clear();
		pressList.clear();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					didWindowQuitFlag = true;
					break;
				}
				case SDL_WINDOWEVENT: {
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
						didWindowSizeChangeFlag = true;
					}
					break;
				}
				case SDL_MOUSEMOTION: {
					mousePos.x = event.motion.x;
					mousePos.y = event.motion.y;
					mouseMoveDelta.x += event.motion.xrel;
					mouseMoveDelta.y += event.motion.yrel;
					break;
				}
				case SDL_MOUSEWHEEL: {
					mouseScrollDelta += event.wheel.y;
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					MouseButton button {};
					switch (event.button.button) {
						case SDL_BUTTON_LEFT:
							mouseLeftDown = true;
							button = MouseButton::Left;
							break;
						case SDL_BUTTON_MIDDLE:
							mouseMiddleDown = true;
							button = MouseButton::Middle;
							break;
						case SDL_BUTTON_RIGHT:
							mouseRightDown = true;
							button = MouseButton::Right;
							break;
					}
					const Ivec2 pos = {event.button.x, event.button.y};
					clickList.push_back({button, pos});
					break;
				}
				case SDL_MOUSEBUTTONUP: {
					MouseButton button {};
					switch (event.button.button) {
						case SDL_BUTTON_LEFT:
							mouseLeftDown = false;
							button = MouseButton::Left;
							break;
						case SDL_BUTTON_MIDDLE:
							mouseMiddleDown = false;
							button = MouseButton::Middle;
							break;
						case SDL_BUTTON_RIGHT:
							mouseRightDown = false;
							button = MouseButton::Right;
							break;
					}
					(void) button; // TODO
					break;
				}
				case SDL_KEYDOWN: {
					std::optional<Key> key;
					switch (event.key.keysym.sym) {
						case SDLK_w:
							key = Key::W;
							keyPressed[Key::W] = true;
							break;
						case SDLK_a:
							key = Key::A;
							keyPressed[Key::A] = true;
							break;
						case SDLK_s:
							key = Key::S;
							keyPressed[Key::S] = true;
							break;
						case SDLK_d:
							key = Key::D;
							keyPressed[Key::D] = true;
							break;
						case SDLK_SPACE:
							key = Key::SPACE;
							keyPressed[Key::SPACE] = true;
							break;
						case SDLK_ESCAPE:
							key = Key::ESCAPE;
							keyPressed[Key::ESCAPE] = true;
							break;
						case SDLK_k:
							key = Key::K;
							keyPressed[Key::K] = true;
							break;
					}
					if (key) {
						pressList.push_back(*key);
					}
					break;
				}
				case SDL_KEYUP: {
					std::optional<Key> key;
					switch (event.key.keysym.sym) {
						case SDLK_w:
							key = Key::W;
							keyPressed[Key::W] = false;
							break;
						case SDLK_a:
							key = Key::A;
							keyPressed[Key::A] = false;
							break;
						case SDLK_s:
							key = Key::S;
							keyPressed[Key::S] = false;
							break;
						case SDLK_d:
							key = Key::D;
							keyPressed[Key::D] = false;
							break;
						case SDLK_SPACE:
							key = Key::SPACE;
							keyPressed[Key::SPACE] = false;
							break;
						case SDLK_ESCAPE:
							key = Key::ESCAPE;
							keyPressed[Key::ESCAPE] = false;
							break;
						case SDLK_k:
							key = Key::K;
							keyPressed[Key::K] = false;
							break;
					}
					(void) key; // TODO
					break;
				}
			}
		}
		if (mouseMoveDelta.x || mouseMoveDelta.y) {
			didMouseMoveFlag = true;
		}
		if (mouseScrollDelta) {
			didMouseScrollFlag = true;
		}
#endif // HEADLESS
	}

	Seconds getDeltaTime()
	{
#ifndef HEADLESS
		return (float) (ticksNow - ticksLast) / 1000;
#else
		return {};
#endif // HEADLESS
	}

	Seconds getRunningTime()
	{
#ifndef HEADLESS
		return (float) (ticksNow - ticksBegin) / 1000;
#else
		return {};
#endif // HEADLESS
	}

	Ivec2 getMousePosition()
	{
		return mousePos;
	}

	bool isMouseButtonPressed(MouseButton button)
	{
		switch (button)
		{
			case MouseButton::Left: return mouseLeftDown;
			case MouseButton::Middle: return mouseMiddleDown;
			case MouseButton::Right: return mouseRightDown;
		}
		return false;
	}

	bool isKeyPressed(Key key)
	{
		if (keyPressed.contains(key)) {
			return keyPressed.at(key);
		}
		return false;
	}

	Ivec2 getWindowSize()
	{
		return windowSize;
	}

	float getWindowRatio()
	{
		if (windowSize.x == 0 || windowSize.y == 0) {
			return 1;
		}
		return (float) windowSize.x / (float) windowSize.y;
	}

	bool didMouseMove()
	{
		return didMouseMoveFlag;
	}
	
	bool didMouseScroll()
	{
		return didMouseScrollFlag;
	}
	
	bool didWindowSizeChange()
	{
		return didWindowSizeChangeFlag;
	}

	bool didWindowQuit()
	{
		return didWindowQuitFlag;
	}

	void quit()
	{
		didWindowQuitFlag = true;
	}

	Vec2 getMouseMoveDelta()
	{
		return mouseMoveDelta;
	}
	
	float getMouseScrollDelta()
	{
		return mouseScrollDelta;
	}

	const std::vector<std::pair<MouseButton, Ivec2>> &getClickList()
	{
		return clickList;
	}
	
	const std::vector<Key> &getPressList()
	{
		return pressList;
	}

}
