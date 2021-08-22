#include "SFMLShipEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

#include <map>

using namespace Melee;

namespace
{
    const std::map<ShipEntity::VisualType, std::string> kShipAssetNames
        {
			{ ShipEntity::VisualType::Androsynth, 				"guardian-big-000" },
			{ ShipEntity::VisualType::AndrosynthBlazer, 		"blazer-big-000" },
            { ShipEntity::VisualType::Human, 					"cruiser-big-000" },
        };
}

SFMLShipEntityRenderer::SFMLShipEntityRenderer(ShipEntity& entity)
    : m_entity(entity)
{
    updateTexture();
}

void SFMLShipEntityRenderer::updateTexture()
{
    m_cachedVisualType = m_entity.visualType();
    m_shipImage = SFMLAssetLoader::Instance().getTexture(kShipAssetNames.at(m_cachedVisualType));

    m_sprite.setTexture(*m_shipImage->texture);
    m_sprite.setTextureRect(m_shipImage->region);
    m_sprite.setOrigin(ToOrigin(m_shipImage->region.width, m_shipImage->region.height));
}

void SFMLShipEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto playerHeading = m_entity.heading();
    const auto playerPos = m_entity.position();
    const auto playerRadius = m_entity.radius();

    if (m_entity.visualType() != m_cachedVisualType)
        updateTexture();

    m_sprite.setRotation(ToDegrees(playerHeading));
    m_sprite.setScale(ToScaleFactor(playerRadius, m_shipImage->region.width, m_shipImage->region.height));
    m_sprite.setPosition(ToSFMLVector(playerPos));

    renderer.draw(m_sprite);
}
