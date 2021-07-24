#include "SFMLRenderer.hpp"

#include "Engine/Engine.hpp"

#include <array>
#include <algorithm>
#include <unordered_map>
#include <utility>

using namespace Melee;

namespace
{
    // FIXME: User key mappings
    const std::unordered_map<sf::Keyboard::Key, PlayerEntity::KeyEvent> kPlayer1Keys =
    {
        { sf::Keyboard::Key::W,       PlayerEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::A,       PlayerEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::S,       PlayerEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::D,       PlayerEntity::KeyEvent::RotateRight },
    };
    const std::unordered_map<sf::Keyboard::Key, PlayerEntity::KeyEvent> kPlayer2Keys =
    {
        { sf::Keyboard::Key::Up,      PlayerEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::Left,    PlayerEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::Down,    PlayerEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::Right,   PlayerEntity::KeyEvent::RotateRight },
    };

   const sf::Color kShipColours[] =
    {
        { 255,   0,   0 },
        {   0, 255,   0 },
        { 255,   0, 255 },
    };

    sf::Vector2f ToSFML(const Vector2d& v)
    {
        return sf::Vector2f{ v.x, v.y };
    }
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

    constexpr auto kTargetFPS = 60;
    constexpr auto kMillisecondsPerFrame = 1000 / kTargetFPS;

	while (window.isOpen())
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

        m_engine.update(kMillisecondsPerFrame);

        window.clear();
        renderEntites(window);
        window.display();

        sf::sleep(sf::milliseconds(kMillisecondsPerFrame));
    }

    return 0;
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

void SFMLRenderer::renderEntites(sf::RenderWindow& window)
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
                const auto playerHeading = playerEntity->heading();
                const auto playerPos = playerEntity->position() / currentScaleFactor();
                const auto playerRadius = playerEntity->properties().radius_km / currentScaleFactor();

                static const auto kRotateLeft90 = RotationMatrix(-90);
                static const auto kRotateRight90 = RotationMatrix(90);

                sf::ConvexShape s;
                s.setFillColor(kShipColours[playerIndex % std::size(kShipColours)]);
                s.setPointCount(3);
                s.setPoint(0, ToSFML(playerPos + playerHeading * playerRadius));
                s.setPoint(1, ToSFML(playerPos + kRotateLeft90 * playerHeading * playerRadius));
                s.setPoint(2, ToSFML(playerPos + kRotateRight90 * playerHeading * playerRadius));

                window.draw(s);
                break;
            }

            case Entity::Type::Planet:
            {
                const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);

                const auto planetPos = planetEntity->position() / currentScaleFactor();
                const auto planetRadius = planetEntity->properties().radius_km / currentScaleFactor();

                sf::CircleShape p;
                p.setFillColor(sf::Color(100, 100, 200));
                p.setOutlineColor(sf::Color(100, 100, 100));
                p.setOutlineThickness(2);
                p.setRadius(planetRadius);
                p.setPosition(ToSFML(planetPos - Vector2d{ planetRadius , planetRadius }));

                window.draw(p);
                break;
            }
        }
    }
}
