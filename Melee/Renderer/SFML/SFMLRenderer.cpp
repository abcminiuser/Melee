#include "SFMLRenderer.hpp"
#include "SFMLRenderable.hpp"
#include "SFMLUtils.hpp"

#include "Entities/SFMLAsteroidEntityRenderer.hpp"
#include "Entities/SFMLExhaustEntityRenderer.hpp"
#include "Entities/SFMLExplosionEntityRenderer.hpp"
#include "Entities/SFMLPlanetEntityRenderer.hpp"
#include "Entities/SFMLPlayerEntityRenderer.hpp"
#include "Entities/SFMLProjectileEntityRenderer.hpp"

#include "Engine/Engine.hpp"

#include <unordered_map>

using namespace Melee;

namespace
{
    // FIXME: User key mappings
    const std::unordered_map<sf::Keyboard::Key, PlayerEntity::KeyEvent> kPlayer1Keys =
    {
        { sf::Keyboard::Key::W,          PlayerEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::A,          PlayerEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::S,          PlayerEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::D,          PlayerEntity::KeyEvent::RotateRight },
        { sf::Keyboard::Key::Tilde,      PlayerEntity::KeyEvent::FirePrimary },
        { sf::Keyboard::Key::Tab,        PlayerEntity::KeyEvent::FireSpecial },
    };
    const std::unordered_map<sf::Keyboard::Key, PlayerEntity::KeyEvent> kPlayer2Keys =
    {
        { sf::Keyboard::Key::Up,         PlayerEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::Left,       PlayerEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::Down,       PlayerEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::Right,      PlayerEntity::KeyEvent::RotateRight },
        { sf::Keyboard::Key::RControl,   PlayerEntity::KeyEvent::FirePrimary },
        { sf::Keyboard::Key::RShift,     PlayerEntity::KeyEvent::FireSpecial },
    };

    constexpr auto kTargetFPS = 60;
    constexpr auto kMillisecondsPerFrame = 1000 / kTargetFPS;
}

SFMLRenderer::SFMLRenderer(Engine& engine)
    : m_engine(engine)
{

}

int SFMLRenderer::runModal()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Melee");
    window.setFramerateLimit(kTargetFPS);

    while (window.isOpen())
    {
        window.clear();

        m_engine.update(kMillisecondsPerFrame);

        updatePlayfieldViewport();
        window.setView(m_playfieldView);

        renderEntities(window);
        processEvents(window);

        window.display();
    }

    return 0;
}

void SFMLRenderer::updatePlayfieldViewport()
{
    const auto playfieldSize = m_engine.getPlayfieldSize();

    const auto minViewportSize = std::min(playfieldSize.x, playfieldSize.y) / 2;
    const auto viewportPadding = Vector2d{ playfieldSize.x * .1f, playfieldSize.y * .1f };

    auto playerBoundingBox = m_engine.getPlayersBoundingBox();

    // We want some padding around the players, so they aren't at the extreme edges of the view.
    playerBoundingBox.inflate(viewportPadding);

    // We want a rectangular viewport that's at least as large as the minimum size, but the larger
    // of the two bounding box axis.
    auto viewportSize = std::max(playerBoundingBox.size.x, playerBoundingBox.size.y);
    viewportSize = std::max(viewportSize, minViewportSize);

    m_playfieldView.setCenter(playerBoundingBox.origin.x + (playerBoundingBox.size.x / 2), playerBoundingBox.origin.y + (playerBoundingBox.size.y / 2));
    m_playfieldView.setSize(viewportSize, viewportSize);
}

void SFMLRenderer::processEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }

            case sf::Event::KeyPressed:
            case sf::Event::KeyReleased:
            {
                handleKey(event.key.code, event.type == sf::Event::KeyPressed);
                break;
            }
        }
    }
}

void SFMLRenderer::handleKey(sf::Keyboard::Key key, bool down)
{
    for (const auto& entity : m_engine.getEntities(Entity::Type::Player))
    {
        const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);
        const auto& playerKeyMap = playerEntity->index() == 0 ? kPlayer1Keys : kPlayer2Keys;

        if (playerKeyMap.count(key))
            playerEntity->handleKey(playerKeyMap.at(key), down);
    }
}

void SFMLRenderer::renderEntities(sf::RenderTarget& target)
{
    for (const auto& entity : m_engine.getEntities())
    {
        auto& rendererContext = entity->rendererContext();
        if (!rendererContext)
            rendererContext = createEntityRenderContext(entity);

        rendererContext->render(target);
    }
}

std::shared_ptr<RenderContext> SFMLRenderer::createEntityRenderContext(const std::shared_ptr<Entity>& entity)
{
    switch (entity->type())
    {
        case Entity::Type::Asteroid:
            return CreateEntityRenderContext<AsteroidEntity, SFMLAsteroidEntityRenderer>(entity);

        case Entity::Type::Exhaust:
            return CreateEntityRenderContext<ExhaustEntity, SFMLExhaustEntityRenderer>(entity);

        case Entity::Type::Explosion:
            return CreateEntityRenderContext<ExplosionEntity, SFMLExplosionEntityRenderer>(entity);

        case Entity::Type::Planet:
            return CreateEntityRenderContext<PlanetEntity, SFMLPlanetEntityRenderer>(entity);

        case Entity::Type::Player:
            return CreateEntityRenderContext<PlayerEntity, SFMLPlayerEntityRenderer>(entity);

        case Entity::Type::Projectile:
            return CreateEntityRenderContext<ProjectileEntity, SFMLProjectileEntityRenderer>(entity);
    }

    throw std::runtime_error("Failed to create render context for entity!");
}
