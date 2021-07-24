#include "SFMLRenderer.hpp"

#include "Engine/Engine.hpp"

#include <array>
#include <algorithm>
#include <unordered_map>
#include <utility>

using namespace Melee;

namespace
{
    const sf::Color HSVColor(float H, float S, float V)
    {
        if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0)
            throw std::out_of_range("Invalid range");

        float s = S / 100;
        float v = V / 100;
        float C = s * v;
        float X = C * (1 - std::abs(std::fmod(H / 60.0, 2) - 1));
        float m = v - C;

        uint8_t c = static_cast<uint8_t>((C + m) * 255);
        uint8_t x = static_cast<uint8_t>((X + m) * 255);

        if (H >= 0 && H < 60)
            return { c, x, 0 };
        else if (H >= 60 && H < 120)
            return { x, c, 0 };
        else if (H >= 120 && H < 180)
            return { 0, c, x };
        else if (H >= 180 && H < 240)
            return { 0, x, c };
        else if (H >= 240 && H < 300)
            return { x, 0, c };
        else
            return { c, 0, x };
    }

    sf::Vector2f ToSFML(const Vector2d& v)
    {
        return sf::Vector2f{ v.x, v.y };
    }

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

   const sf::Color kExhaustColours[] =
   {
        HSVColor(50, 100, 100),
        HSVColor(40, 85, 80),
        HSVColor(30, 85, 60),
        HSVColor(20, 85, 40),
        HSVColor(10, 85, 35),
        HSVColor(0, 85, 20),
   };
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
                p.setPosition(ToSFML(planetPos));
                p.setOrigin(planetRadius, planetRadius);

                window.draw(p);
                break;
            }

            case Entity::Type::Exhaust:
            {
                const auto exhaustEntity = std::dynamic_pointer_cast<ExhaustEntity>(entity);

                const auto exhaustPos = exhaustEntity->position() / currentScaleFactor();
                const auto exhaustAge = exhaustEntity->age() / static_cast<float>(exhaustEntity->properties().maxAge);

                sf::CircleShape p;
                p.setFillColor(kExhaustColours[static_cast<int>(exhaustAge * std::size(kExhaustColours))]);
                p.setRadius(1);
                p.setPosition(ToSFML(exhaustPos));
                p.setOrigin(1, 1);

                window.draw(p);
                break;
            }
        }
    }
}
