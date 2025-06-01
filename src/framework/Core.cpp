#include <cassert>
#include <ctime>
#include <cstdlib>

#ifndef HEADLESS
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif // HEADLESS

#include "Core.hpp"
#include "Input.hpp"
#include "Behaviour.hpp"
#include "Entity.hpp"
#include "ComponentManager.hpp"
#include "Renderer.hpp"
#include "Utils.hpp"
#include "UserEvent.hpp"
#include "Fonts.hpp"
#include "Selector.hpp"

Entity::Id Core::ids = 0;
std::unordered_map<Entity::Id, std::unique_ptr<Entity>> Core::entities;
std::unordered_set<Entity::Id> Core::entitiesToRemove;

#ifndef HEADLESS

SDL_Window *window;

#endif // HEADLESS

void Core::initialize()
{
	std::srand(static_cast<Uint>(time(nullptr)));

#ifndef HEADLESS

	#ifdef SDL_HINT_WINDOWS_DPI_AWARENESS
		assert(SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2") == SDL_TRUE);
	#endif

	#ifdef SDL_HINT_WINDOWS_DPI_SCALING
		assert(SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "0") == SDL_TRUE);
	#endif

	assert(SDL_Init(SDL_INIT_VIDEO) == 0);

	assert(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) == 0);

	//assert(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) == 0);
	//assert(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4) == 0);

	assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) == 0);
	assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == 0);

	constexpr int windowInitW = 1080, windowInitH = 720;
	window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowInitW, windowInitH, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
	//window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowInitW, windowInitH, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);
	//window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowInitW, windowInitH, SDL_WINDOW_OPENGL);
	assert(window);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	assert(context);

	assert(SDL_GL_SetSwapInterval(1) == 0);

	assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0); // TODO
	Fonts::init();
	Input::initialize();
	Renderer::init();

#endif // HEADLESS
}

void Core::loop()
{
#ifndef HEADLESS

	Uint begin = SDL_GetTicks();
	Uint frames = 0;

	while (!Input::didWindowQuit())
	{
		ComponentManager::update();
		Core::clean();

		Input::update();
		Selector::update();
		Behaviour::earlyUpdateAll();
		Behaviour::updateAll();
		Behaviour::lateUpdateAll();
		UserEvent::update();
		Renderer::render();

		Core::swapFrame();

		if (++frames % 25 == 0) {
			const float ms = (float) (SDL_GetTicks() - begin) / frames;
			const float fps = 1000 / ms;
			SDL_SetWindowTitle(window, ("fps: " + std::to_string(fps) + " ms: " + std::to_string(ms)).c_str());
		}
	}

	Fonts::destroy();

	// TODO: destroy SDL
#endif // HEADLESS
}


//make shared_ptr possible
Entity &Core::createEntity(bool pinned)
{
	auto entity = std::make_unique<Entity>(ids++, pinned);
	Entity *result = entity.get();
	entities[entity->id] = std::move(entity);
	return *result;
}

void Core::destroyEntity(const Entity &entity)
{
	ComponentManager::removeAllComponents(entity);
	entitiesToRemove.insert(entity.id);
}

void Core::destroyAllEntities()
{
	for (const auto &[id, entity] : entities) {
		if (!entity->pinned) {
			destroyEntity(*entity);
		}
	}
}

void Core::clean()
{
	if (!entitiesToRemove.empty()) {
		std::erase_if(entities, [](auto &pair) {
			return entitiesToRemove.contains(pair.second->id);
		});
		entitiesToRemove.clear();
	}
}

void Core::swapFrame()
{
#ifndef HEADLESS
	SDL_GL_SwapWindow(window);
#endif // HEADLESS
}
