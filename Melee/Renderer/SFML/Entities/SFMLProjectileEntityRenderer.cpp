#include "SFMLProjectileEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLProjectileEntityRenderer::SFMLProjectileEntityRenderer(ProjectileEntity& entity)
    : m_entity(entity)
    , m_projectileImage(SFMLAssetLoader::Instance().getTexture("missile"))
{
    m_sprite.setTexture(*m_projectileImage.texture);
    m_sprite.setTextureRect(m_projectileImage.region);
    m_sprite.setOrigin(sf::Vector2f{ m_projectileImage.region.width / 2.0f, m_projectileImage.region.height / 2.0f });
}

void SFMLProjectileEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto projectileHeading = m_entity.heading();
    const auto projectilePos = m_entity.position();
    const auto projectileRadius = m_entity.properties().radius_km;

    m_sprite.setRotation(ToDegrees(projectileHeading));
    m_sprite.setScale(sf::Vector2f{ projectileRadius * 2 / m_projectileImage.region.width, projectileRadius * 2 / m_projectileImage.region.height });
    m_sprite.setPosition(ToSFMLVector(projectilePos));

    renderer.draw(m_sprite);
}
