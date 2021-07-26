#include "SFMLProjectileEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLProjectileEntityRenderer::SFMLProjectileEntityRenderer(ProjectileEntity& entity)
    : m_entity(entity)
{

}

void SFMLProjectileEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto projectilePos = m_entity.position();
    const auto projectileRadius = m_entity.properties().radius_km;

    m_shape.setRadius(projectileRadius);
    m_shape.setOrigin(projectileRadius, projectileRadius);

    m_shape.setFillColor(sf::Color::White);
    m_shape.setPosition(ToSFMLVector(projectilePos));

    renderer.draw(m_shape);
}
