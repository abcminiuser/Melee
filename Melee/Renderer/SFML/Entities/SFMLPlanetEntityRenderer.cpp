#include "SFMLPlanetEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlanetEntityRenderer::SFMLPlanetEntityRenderer(PlanetEntity& entity)
    : m_entity(entity)
    , m_planetImage(SFMLAssetLoader::Instance().getTexture("planet"))
{
    m_sprite.setTexture(*m_planetImage.texture);
    m_sprite.setTextureRect(m_planetImage.region);
    m_sprite.setOrigin(sf::Vector2f{ m_planetImage.region.width / 2.0f, m_planetImage.region.height / 2.0f });
}

void SFMLPlanetEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto planetRadius = m_entity.properties().radius_km;
    const auto planetPos = m_entity.position();

    m_sprite.setScale(sf::Vector2f{ planetRadius * 2 / m_planetImage.region.width, planetRadius * 2 / m_planetImage.region.height });
    m_sprite.setPosition(ToSFMLVector(planetPos));

    renderer.draw(m_sprite);
}
