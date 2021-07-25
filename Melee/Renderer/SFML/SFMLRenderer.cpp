#include "SFMLRenderer.hpp"
#include "SFMLRenderable.hpp"
#include "SFMLUtils.hpp"

#include "Entities/SFMLAsteroidEntityRenderer.hpp"
#include "Entities/SFMLExhaustEntityRenderer.hpp"
#include "Entities/SFMLPlanetEntityRenderer.hpp"
#include "Entities/SFMLPlayerEntityRenderer.hpp"

#include "Engine/Engine.hpp"

#include <unordered_map>
#include <chrono>

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
    m_engine.setCollisionCallback(
        [this](const std::shared_ptr<Entity>& e1, const std::shared_ptr<Entity>& e2)
        {
            // TODO: Pixel/geometry accurate collision check
            return true;
        });
}

int SFMLRenderer::runModal()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Melee");
    window.setFramerateLimit(kTargetFPS);

    while (window.isOpen())
    {
        window.clear();

        m_engine.update(kMillisecondsPerFrame);

        processEvents(window);
        renderEntities(window);

        window.display();
    }

    return 0;
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

void SFMLRenderer::renderEntities(sf::RenderWindow& window)
{
    const auto scaleFactor = currentScaleFactor();

    for (const auto& entity : m_engine.getEntities())
    {
        auto& rendererContext = entity->rendererContext();
        if (!rendererContext)
            rendererContext = createRenderContext(entity);

        rendererContext->render(window, scaleFactor);
    }
}

std::shared_ptr<RenderContext> SFMLRenderer::createRenderContext(const std::shared_ptr<Entity>& entity)
{
    switch (entity->type())
    {
        case Entity::Type::Player:
        {
            const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);
            return std::make_shared<SFMLPlayerEntityRenderer>(*playerEntity);
        }

        case Entity::Type::Planet:
        {
            const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);
            return std::make_shared<SFMLPlanetEntityRenderer>(*planetEntity);
        }

        case Entity::Type::Exhaust:
        {
            const auto exhaustEntity = std::dynamic_pointer_cast<ExhaustEntity>(entity);
            return std::make_shared<SFMLExhaustEntityRenderer>(*exhaustEntity);
        }

        case Entity::Type::Asteroid:
        {
            const auto asteroidEntity = std::dynamic_pointer_cast<AsteroidEntity>(entity);
            return std::make_shared<SFMLAsteroidEntityRenderer>(*asteroidEntity);
        }
    }

    throw std::runtime_error("Failed to create render context for entity!");
}
