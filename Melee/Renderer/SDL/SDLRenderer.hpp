#pragma once

#include <memory>

#include <SDL.h>

namespace Melee
{
    class Engine;

    class SDLRenderer final
    {
    public:
        explicit                        SDLRenderer(Engine& engine);
        ~SDLRenderer();

        int                             runModal();

    private:
        void                            renderEntities();
        void                            handleKey(SDL_Keycode key, bool down);

    private:
        Engine& m_engine;

        std::shared_ptr<SDL_Window>     m_window = nullptr;
        SDL_Renderer*                   m_renderer = nullptr;
    };
}