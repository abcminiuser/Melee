#include "Renderer/SDL/SDLRenderer.hpp"
#include "Engine/Engine.hpp"

#include <unordered_map>
#include <array>
#include <algorithm>

#include <SDL2_gfxPrimitives.h>

using namespace Melee;

SDLRenderer::SDLRenderer(Engine& engine)
    : m_engine(engine)
{
    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow(
            "Melee",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1024,
            768,
            0
        );

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

SDLRenderer::~SDLRenderer()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

int SDLRenderer::runModal()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    bool run = true;
    while (run)
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_renderer);

        renderEntities();

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    run = false;
                    break;
                }

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                {
                    handleKey(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                    break;
                }
            }
        }

        SDL_RenderPresent(m_renderer);

        constexpr auto kTargetFPS = 60;
        constexpr auto kMillisecondsPerFrame = 1000 / kTargetFPS;

        SDL_Delay(kMillisecondsPerFrame);

        m_engine.update(kMillisecondsPerFrame);
    }

    return 0;
}

void SDLRenderer::renderEntities()
{
    for (const auto& entity : m_engine.getEntities())
    {
        // FIXME: Proper entity rendering
        switch (entity->type())
        {
            case Entity::Type::Player:
            {
                const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);

                const int playerIndex = playerEntity->index();
                const auto playerPos = playerEntity->position() / 1000;
                const auto playerHeading = playerEntity->heading();

                static const auto rotateLeft90 = RotationMatrix(-90);
                static const auto rotateRight90 = RotationMatrix(90);

                std::array<Point, 3> shipPoints;
                shipPoints[0] = playerPos + playerHeading * 20;
                shipPoints[1] = playerPos + rotateLeft90 * playerHeading * 5;
                shipPoints[2] = playerPos + rotateRight90 * playerHeading * 5;

                static const auto ToSDLPoint = [](const auto& p) { return SDL_Point{ (int)p.x, (int)p.y };  };
                std::array<SDL_Point, 4> drawPoints = { ToSDLPoint(shipPoints[0]), ToSDLPoint(shipPoints[1]), ToSDLPoint(shipPoints[2]), ToSDLPoint(shipPoints[0]) };

                SDL_SetRenderDrawColor(m_renderer, (playerIndex % 3 == 0) ? 255 : 0, (playerIndex % 3 == 1) ? 255 : 0, (playerIndex % 3 == 2) ? 255 : 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLines(m_renderer, drawPoints.data(), drawPoints.size());
                break;
            }

            case Entity::Type::Planet:
            {
                const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);

                const auto planetPos = planetEntity->position() / 1000;
                const auto planetRadius = planetEntity->radius();

                SDL_SetRenderDrawColor(m_renderer, 180, 180, 180, SDL_ALPHA_OPAQUE);
                circleRGBA(m_renderer, (int)planetPos.x, (int)planetPos.y, (int)(planetRadius / 100), 128, 128, 128, 255);
                break;
            }
        }
    }
}

void SDLRenderer::handleKey(SDL_Keycode key, bool down)
{
    // FIXME: User key mappings
    static const std::unordered_map<SDL_Keycode, PlayerEntity::KeyEvent> kPlayer1Keys =
    {
        { SDLK_w,       PlayerEntity::KeyEvent::Thrust },
        { SDLK_a,       PlayerEntity::KeyEvent::RotateLeft },
        { SDLK_s,       PlayerEntity::KeyEvent::ReverseThrust },
        { SDLK_d,       PlayerEntity::KeyEvent::RotateRight },
    };
    static const std::unordered_map<SDL_Keycode, PlayerEntity::KeyEvent> kPlayer2Keys =
    {
        { SDLK_UP,      PlayerEntity::KeyEvent::Thrust },
        { SDLK_LEFT,    PlayerEntity::KeyEvent::RotateLeft },
        { SDLK_DOWN,    PlayerEntity::KeyEvent::ReverseThrust },
        { SDLK_RIGHT,   PlayerEntity::KeyEvent::RotateRight },
    };

    for (const auto& entity : m_engine.getEntities())
    {
        if (entity->type() != Entity::Type::Player)
            continue;

        const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);
        const auto& playerKeyMap = playerEntity->index() == 0 ? kPlayer1Keys : kPlayer2Keys;

        if (playerKeyMap.count(key))
            playerEntity->handleKey(playerKeyMap.at(key), down);
    }
}
