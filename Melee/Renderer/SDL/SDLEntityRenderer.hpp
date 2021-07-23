#pragma once

#include <SDL.h>

namespace Melee
{
	class Engine;

	namespace SDLEntityRenderer
	{
		void RenderEntities(Engine& engine, SDL_Renderer* renderer, SDL_Rect& area, uint32_t scaleFactor);
		void RenderHUD(Engine& engine, SDL_Renderer* renderer, SDL_Rect& area);
	}
}
