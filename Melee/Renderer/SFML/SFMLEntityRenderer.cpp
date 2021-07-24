#include "SFMLEntityRenderer.hpp"
#include "SFMLUtils.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
	const sf::Color kShipColours[] =
	{
	    { 255,   0,   0 },
	    {   0, 255,   0 },
	    { 255,   0, 255 },
	};

	const sf::Color kExhaustColours[] =
	{
	    HSVColor(50, 100, 100),
	    HSVColor(40, 85, 80),
	    HSVColor(30, 85, 60),
	    HSVColor(20, 85, 40),
	    HSVColor(10, 85, 35),
	    HSVColor(0, 85, 20),
	};

	void RenderPlayerEntity(const std::shared_ptr<PlayerEntity>& entity, sf::RenderWindow& window, uint32_t scaleFactor)
	{
        const int playerIndex = entity->index();
        const auto playerHeading = entity->heading();
        const auto playerPos = entity->position() / scaleFactor;
        const auto playerRadius = entity->properties().radius_km / scaleFactor;

        struct PlayerRenderContext
        {
            sf::ConvexShape s;
        };

        auto rendererContext = entity->getRendererContext<PlayerRenderContext>();
        if (!rendererContext)
        {
            rendererContext = std::make_shared<PlayerRenderContext>();

            rendererContext->s.setFillColor(kShipColours[playerIndex % std::size(kShipColours)]);
            rendererContext->s.setPointCount(3);

            entity->setRendererContext(rendererContext);
        }

        static const auto kRotateLeft90 = RotationMatrix(-90);
        static const auto kRotateRight90 = RotationMatrix(90);

        rendererContext->s.setPoint(0, ToSFML(playerPos + playerHeading * playerRadius));
        rendererContext->s.setPoint(1, ToSFML(playerPos + kRotateLeft90 * playerHeading * playerRadius));
        rendererContext->s.setPoint(2, ToSFML(playerPos + kRotateRight90 * playerHeading * playerRadius));

        window.draw(rendererContext->s);
	}

	void RenderPlanetEntity(const std::shared_ptr<PlanetEntity>& entity, sf::RenderWindow& window, uint32_t scaleFactor)
	{
        const auto planetPos = entity->position() / scaleFactor;
        const auto planetRadius = entity->properties().radius_km / scaleFactor;

        struct PlanetRenderContext
        {
            sf::CircleShape s;
        };

        auto rendererContext = entity->getRendererContext<PlanetRenderContext>();
        if (!rendererContext)
        {
            rendererContext = std::make_shared<PlanetRenderContext>();

            rendererContext->s.setFillColor(sf::Color(100, 100, 200));
            rendererContext->s.setOutlineColor(sf::Color(100, 100, 100));
            rendererContext->s.setOutlineThickness(2);
            rendererContext->s.setRadius(planetRadius);
            rendererContext->s.setPosition(ToSFML(planetPos));
            rendererContext->s.setOrigin(planetRadius, planetRadius);

            entity->setRendererContext(rendererContext);
        }

        window.draw(rendererContext->s);
	}

	void RenderExhaustEntity(const std::shared_ptr<ExhaustEntity>& entity, sf::RenderWindow& window, uint32_t scaleFactor)
	{
        const auto exhaustPos = entity->position() / scaleFactor;
        const auto exhaustAge = entity->age() / static_cast<float>(entity->properties().maxAge);

        struct ExhaustRenderContext
        {
            sf::CircleShape s;
        };

        auto rendererContext = entity->getRendererContext<ExhaustRenderContext>();
        if (!rendererContext)
        {
            rendererContext = std::make_shared<ExhaustRenderContext>();

            rendererContext->s.setFillColor(kExhaustColours[static_cast<int>(exhaustAge * std::size(kExhaustColours))]);
            rendererContext->s.setRadius(1);
            rendererContext->s.setPosition(ToSFML(exhaustPos));
            rendererContext->s.setOrigin(1, 1);

            entity->setRendererContext(rendererContext);
        }

        window.draw(rendererContext->s);
	}
}

void SFMLEntityRenderer::RenderEntites(Engine& engine, sf::RenderWindow& window, uint32_t scaleFactor)
{
    for (const auto& entity : engine.getEntities())
    {
        // FIXME: Proper entity rendering
        switch (entity->type())
        {
            case Entity::Type::Player:
            {
                const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);
                RenderPlayerEntity(playerEntity, window, scaleFactor);
                break;
            }

            case Entity::Type::Planet:
            {
    	        const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);
    	        RenderPlanetEntity(planetEntity, window, scaleFactor);
                break;
            }

            case Entity::Type::Exhaust:
            {
				const auto exhaustEntity = std::dynamic_pointer_cast<ExhaustEntity>(entity);
				RenderExhaustEntity(exhaustEntity, window, scaleFactor);
                break;
            }
        }
    }
}
