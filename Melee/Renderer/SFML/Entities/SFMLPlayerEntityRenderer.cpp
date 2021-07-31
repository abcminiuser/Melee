#include "SFMLPlayerEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlayerEntityRenderer::SFMLPlayerEntityRenderer(PlayerEntity& entity)
    : m_entity(entity)
    , m_shipImage(SFMLAssetLoader::Instance().getTexture("ship"))
{
    m_sprite.setTexture(*m_shipImage.texture);
    m_sprite.setTextureRect(m_shipImage.region);
    m_sprite.setOrigin(sf::Vector2f{ m_shipImage.region.width / 2.0f, m_shipImage.region.height / 2.0f });
}

void SFMLPlayerEntityRenderer::render(sf::RenderTarget& renderer)
{
    const int playerIndex = m_entity.index();
    const auto playerHeading = m_entity.heading();
    const auto playerPos = m_entity.position();
    const auto playerRadius = m_entity.properties().radius_km;

    m_sprite.setRotation(ToDegrees(playerHeading));
    m_sprite.setScale(sf::Vector2f{ playerRadius * 2 / m_shipImage.region.width, playerRadius * 2 / m_shipImage.region.height });
    m_sprite.setPosition(ToSFMLVector(playerPos));

    renderer.draw(m_sprite);
}
