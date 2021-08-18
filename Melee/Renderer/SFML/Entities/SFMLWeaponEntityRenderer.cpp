#include "SFMLWeaponEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

#include <map>

using namespace Melee;

namespace
{
    const std::map<WeaponEntity::VisualType, std::string> kWeaponAssetNames
        {
            { WeaponEntity::VisualType::HumanMissile, "saturn-big-000" },
        };
}

SFMLWeaponEntityRenderer::SFMLWeaponEntityRenderer(WeaponEntity& entity)
    : m_entity(entity)
    , m_weaponImage(SFMLAssetLoader::Instance().getTexture(kWeaponAssetNames.at(entity.visualType())))
{
    m_sprite.setTexture(*m_weaponImage->texture);
    m_sprite.setTextureRect(m_weaponImage->region);
    m_sprite.setOrigin(sf::Vector2f{ m_weaponImage->region.width / 2.0f, m_weaponImage->region.height / 2.0f });
}

void SFMLWeaponEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto projectileHeading = m_entity.heading();
    const auto projectilePos = m_entity.position();
    const auto projectileRadius = m_entity.radius();

    const auto scaleFactor = projectileRadius * 2 / std::max(m_weaponImage->region.width, m_weaponImage->region.height);

    m_sprite.setRotation(ToDegrees(projectileHeading));
    m_sprite.setScale(sf::Vector2f{ scaleFactor, scaleFactor });
    m_sprite.setPosition(ToSFMLVector(projectilePos));

    renderer.draw(m_sprite);
}
