#include "Renderer/SDL/SDLRenderer.hpp"
#include "Renderer/SDL/SDLEntityRenderer.hpp"

#include "Engine/Engine.hpp"

#include <unordered_map>
#include <array>
#include <algorithm>

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

    constexpr auto kWindowWidth = 1024;
    constexpr auto kWindowHeight = 768;

    constexpr auto kKilometersPerPixel = 180;
}

SDLRenderer::SDLRenderer(Engine& engine)
    : m_engine(engine)
{
    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow(
        "Melee",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kWindowWidth,
        kWindowHeight,
        SDL_WINDOW_RESIZABLE
    );

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    m_engine.setCollisionCallback(
        [this](const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2)
        {
            // TODO: Pixel/geometry accurate collision check
            return true;
        });

    handleMetricsUpdate(kWindowWidth, kWindowHeight);

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
    playerProps.mass_kg = 9.718e6;
    playerProps.engineForce_N = 2.5e7;
    playerProps.maxVelocity = 0;
    playerProps.rotation_degPerSec = 100;
    playerProps.maxVelocity = 1000;
    playerProps.maxHealth = 20;
    playerProps.maxEnergy = 10;
    playerProps.radius_km = 1000;

    auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ 200 * kKilometersPerPixel, 200 * kKilometersPerPixel });
    m_engine.addEntity(player1);

    auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ 100 * kKilometersPerPixel, 100 * kKilometersPerPixel });
    m_engine.addEntity(player2);

    PlanetEntity::PlanetProperties planetProps = {};
    planetProps.mass_kg = 5.9736e24 / 500000;
    planetProps.radius_km = 6371;

    auto planet = std::make_shared<PlanetEntity>(planetProps, Point{ 400 * kKilometersPerPixel, 300 * kKilometersPerPixel });
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

        SDLEntityRenderer::RenderEntities(m_engine, m_renderer, m_playfieldArea, kKilometersPerPixel);
        SDLEntityRenderer::RenderHUD(m_engine, m_renderer, m_playerHudArea);

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
