#include "Renderer/SDL/SDLRenderer.hpp"
#include "Engine/Engine.hpp"

#include <unordered_map>
#include <array>
#include <algorithm>

#include <SDL2_gfxPrimitives.h>

using namespace Melee;

namespace
{
    // FIXME: User key mappings
    const std::unordered_map<SDL_Keycode, PlayerEntity::KeyEvent> kPlayer1Keys =
    {
        { SDLK_w,       PlayerEntity::KeyEvent::Thrust },
        { SDLK_a,       PlayerEntity::KeyEvent::RotateLeft },
        { SDLK_s,       PlayerEntity::KeyEvent::ReverseThrust },
        { SDLK_d,       PlayerEntity::KeyEvent::RotateRight },
    };
    const std::unordered_map<SDL_Keycode, PlayerEntity::KeyEvent> kPlayer2Keys =
    {
        { SDLK_UP,      PlayerEntity::KeyEvent::Thrust },
        { SDLK_LEFT,    PlayerEntity::KeyEvent::RotateLeft },
        { SDLK_DOWN,    PlayerEntity::KeyEvent::ReverseThrust },
        { SDLK_RIGHT,   PlayerEntity::KeyEvent::RotateRight },
    };

    SDL_Point ToSDLPoint(const Point& p)
    {
        return SDL_Point{ (int)p.x, (int)p.y };
    };

    constexpr auto kPixelsPerKm = 100;
}

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
        SDL_WINDOW_RESIZABLE
    );

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    m_engine.setCollisionCallback(
        [this](const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2)
        {
            const auto distanceBetweenEntitiesSquared = (e1->position() - e2->position()).lengthSquared();
            const auto minCollisionRadius = e1->properties().radius_km + e2->properties().radius_km;

            // Crude radius check
            if (distanceBetweenEntitiesSquared / kPixelsPerKm > (minCollisionRadius * minCollisionRadius))
                return false;

            // TODO: Pixel/geometry accurate collision check
            return true;
        });

    handleMetricsUpdate(1024, 768);

    setupTestGame();
}

SDLRenderer::~SDLRenderer()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void SDLRenderer::setupTestGame()
{
    PlayerEntity::PlayerProperties playerProps = {};
    playerProps.mass_kg = 90.718e6;
    playerProps.engineForce_N = 2e9;
    playerProps.maxVelocity = 0;
    playerProps.rotation_degPerSec = 100;
    playerProps.maxVelocity = 1000;
    playerProps.maxHealth = 20;
    playerProps.maxEnergy = 10;
    playerProps.radius_km = 1000;

    auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ 200 * 1000, 200 * 1000 });
    m_engine.addEntity(player1);

    auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ 100, 100 });
    m_engine.addEntity(player2);

    PlanetEntity::PlanetProperties planetProps = {};
    planetProps.mass_kg = 5.9736e24 / 5000;
    planetProps.radius_km = 6371;

    auto planet = std::make_shared<PlanetEntity>(planetProps, Point{ 400 * 1000, 300 * 1000 });
    m_engine.addEntity(planet);
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
        renderPlayerHuds();

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

                case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                        handleMetricsUpdate(event.window.data1, event.window.data2);

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
                const auto playerRadius = playerEntity->properties().radius_km;

                static const auto rotateLeft90 = RotationMatrix(-90);
                static const auto rotateRight90 = RotationMatrix(90);

                std::array<SDL_Point, 4> shipPoints;
                shipPoints[0] = ToSDLPoint(playerPos + playerHeading * (playerRadius / kPixelsPerKm));
                shipPoints[1] = ToSDLPoint(playerPos + rotateLeft90 * playerHeading * (playerRadius / kPixelsPerKm));
                shipPoints[2] = ToSDLPoint(playerPos + rotateRight90 * playerHeading * (playerRadius / kPixelsPerKm));
                shipPoints[3] = shipPoints[0];

                SDL_SetRenderDrawColor(m_renderer, (playerIndex % 3 == 0) ? 255 : 0, (playerIndex % 3 == 1) ? 255 : 0, (playerIndex % 3 == 2) ? 255 : 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLines(m_renderer, shipPoints.data(), shipPoints.size());
                break;
            }

            case Entity::Type::Planet:
            {
                const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);

                const auto planetPos = planetEntity->position() / 1000;
                const auto planetRadius = planetEntity->properties().radius_km;

                SDL_SetRenderDrawColor(m_renderer, 180, 180, 180, SDL_ALPHA_OPAQUE);
                circleRGBA(m_renderer, (int)planetPos.x, (int)planetPos.y, (int)(planetRadius / kPixelsPerKm), 128, 128, 128, 255);
                break;
            }
        }
    }
}

void SDLRenderer::renderPlayerHuds()
{
	SDL_SetRenderDrawColor(m_renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, &m_playerHudArea);

#if 0
    const auto& playerEntities = m_engine.getEntities(Entity::Type::Player);

    auto playerRect = SDL_Rect{ m_playerHudArea.x, 0, m_playerHudArea.w, m_playerHudArea.w };
    SDL_SetRenderDrawColor(m_renderer, 250, 250, 150, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, &playerRect);
#endif
}

void SDLRenderer::handleMetricsUpdate(int screenWidth, int screenHeight)
{
    const auto kPlayerHUDWidth = screenWidth / 5;

    m_playfieldArea = SDL_Rect{ 0, 0, screenWidth - kPlayerHUDWidth, screenHeight };
    m_playerHudArea = SDL_Rect{ m_playfieldArea.w, 0, kPlayerHUDWidth, screenHeight };
}

void SDLRenderer::handleKey(SDL_Keycode key, bool down)
{
    for (const auto& entity : m_engine.getEntities(Entity::Type::Player))
    {
        const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);
        const auto& playerKeyMap = playerEntity->index() == 0 ? kPlayer1Keys : kPlayer2Keys;

        if (playerKeyMap.count(key))
            playerEntity->handleKey(playerKeyMap.at(key), down);
    }
}
