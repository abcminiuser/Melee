#include "SFMLPlayerEntityRenderer.hpp"
#include "SFMLUtils.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlayerEntityRenderer::SFMLPlayerEntityRenderer(PlayerEntity& entity)
    : m_entity(entity)
{
    m_shipImage.loadFromFile("Assets\\Ship.png");
    m_shipImageSize = m_shipImage.getSize();

    m_sprite.setTexture(m_shipImage);
    m_sprite.setOrigin(sf::Vector2f{ m_shipImageSize.x / 2.0f, m_shipImageSize.y / 2.0f });
}

void SFMLPlayerEntityRenderer::render(sf::RenderTarget& renderer, uint32_t scaleFactor)
{
    const int playerIndex = m_entity.index();
    const auto playerHeading = m_entity.heading();
    const auto playerPos = m_entity.position() / scaleFactor;
    const auto playerRadius = m_entity.properties().radius_km / scaleFactor;

    if (m_lastHeading != playerHeading || m_lastScaleFactor != scaleFactor)
    {
        m_lastHeading = playerHeading;
        m_lastScaleFactor = scaleFactor;

        m_sprite.setScale(sf::Vector2f{ playerRadius * 2 / m_shipImageSize.x, playerRadius * 2 / m_shipImageSize.y });
        m_sprite.setRotation(ToDegrees(playerHeading));
    }

    m_sprite.setPosition(ToSFML(playerPos));

    renderer.draw(m_sprite);
}
