#include "SDLEntityRenderer.hpp"

#include "Engine/Engine.hpp"

#include <SDL2_gfxPrimitives.h>

#include <array>

using namespace Melee;

namespace
{
    SDL_Point ToSDLPoint(const Point& p)
    {
        return SDL_Point{ (int)p.x, (int)p.y };
    };

    const std::tuple<int, int, int> kShipColours[] =
        {
            { 255,   0,   0 },
            {   0, 255,   0 },
            { 255,   0, 255 },
        };

    constexpr auto kHudPlayerGap = 10;

    const auto kRotateLeft90 = RotationMatrix(-90);
    const auto kRotateRight90 = RotationMatrix(90);
}

void SDLEntityRenderer::RenderEntities(Engine& engine, SDL_Renderer* renderer, SDL_Rect& area, uint32_t scaleFactor)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &area);

    for (const auto& entity : engine.getEntities())
    {
        // FIXME: Proper entity rendering
        switch (entity->type())
        {
            case Entity::Type::Player:
            {
                const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);

                const int playerIndex = playerEntity->index();
                const auto playerHeading = playerEntity->heading();
                const auto playerPos = playerEntity->position() / scaleFactor;
                const auto playerRadius = playerEntity->properties().radius_km / scaleFactor;

                std::array<SDL_Point, 4> shipPoints;
                shipPoints[0] = ToSDLPoint(playerPos + playerHeading * (playerRadius));
                shipPoints[1] = ToSDLPoint(playerPos + kRotateLeft90 * playerHeading * playerRadius);
                shipPoints[2] = ToSDLPoint(playerPos + kRotateRight90 * playerHeading * playerRadius);
                shipPoints[3] = shipPoints[0];

                const auto& [r, g, b] = kShipColours[playerIndex % std::size(kShipColours)];

                SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLines(renderer, shipPoints.data(), shipPoints.size());
                break;
            }

            case Entity::Type::Planet:
            {
                const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);

                const auto planetPos = planetEntity->position() / scaleFactor;
                const auto planetRadius = planetEntity->properties().radius_km / scaleFactor;

                SDL_SetRenderDrawColor(renderer, 180, 180, 180, SDL_ALPHA_OPAQUE);
                circleRGBA(renderer, (int)planetPos.x, (int)planetPos.y, (int)(planetRadius), 128, 128, 128, 255);
                break;
            }
        }
    }
}

void SDLEntityRenderer::RenderHUD(Engine& engine, SDL_Renderer* renderer, SDL_Rect& area)
{
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &area);

    const auto& playerEntities = engine.getEntities(Entity::Type::Player);

    for (size_t i = 0; i < playerEntities.size(); i++)
    {
        auto playerRect = SDL_Rect{ area.x, kHudPlayerGap + area.y + (area.w + kHudPlayerGap) * (int)i, area.w, area.w };

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &playerRect);
    }
}
