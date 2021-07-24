#include "SFMLRenderer.hpp"
#include "SFMLEntityRenderer.hpp"
#include "SFMLUtils.hpp"

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
        SFMLEntityRenderer::RenderEntites(m_engine, window, currentScaleFactor());
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
