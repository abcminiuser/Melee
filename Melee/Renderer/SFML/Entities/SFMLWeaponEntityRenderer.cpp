#include "SFMLWeaponEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

#include <array>
#include <map>

using namespace Melee;

namespace
{
    const std::map<WeaponEntity::VisualType, std::string> kWeaponAssetNames
        {
            { WeaponEntity::VisualType::Laser,        "empty" },
            { WeaponEntity::VisualType::HumanMissile, "saturn-big-000" },
        };
}

SFMLWeaponEntityRenderer::SFMLWeaponEntityRenderer(WeaponEntity& entity)
    : m_entity(entity)
    , m_weaponImage(SFMLAssetLoader::Instance().getTexture(kWeaponAssetNames.at(entity.visualType())))
{
    m_sprite.setTexture(*m_weaponImage->texture);
    m_sprite.setTextureRect(m_weaponImage->region);
    m_sprite.setOrigin(ToOrigin(m_weaponImage->region.width, m_weaponImage->region.height));
}

void SFMLWeaponEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto weaponHeading = m_entity.heading();
    const auto weaponPos = m_entity.position();
    const auto weaponRadius = m_entity.radius();

    if (m_entity.visualType() == WeaponEntity::VisualType::Laser)
    {
        const auto parentPos = m_entity.parentEntity()->position();

        std::array<sf::Vertex, 2> line;
        line[0].position = ToSFMLVector(parentPos);
        line[0].color = sf::Color::White;
        line[1].position = ToSFMLVector(weaponPos);
        line[1].color = sf::Color::White;

        renderer.draw(line.data(), line.size(), sf::Lines);
    }
    else
    {
        m_sprite.setRotation(ToDegrees(weaponHeading));
        m_sprite.setScale(ToScaleFactor(weaponRadius, m_weaponImage->region.width, m_weaponImage->region.height));
        m_sprite.setPosition(ToSFMLVector(weaponPos));

        renderer.draw(m_sprite);
    }
}
