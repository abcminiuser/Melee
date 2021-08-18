#include "SFMLPlanetEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    const std::map<PlanetEntity::VisualType, std::string> kPlanetAssetNames
        {
			{ PlanetEntity::VisualType::Acid,				"Acid-big-000" },
			{ PlanetEntity::VisualType::Alkali,				"Alkali-big-000" },
			{ PlanetEntity::VisualType::Auric,				"Auric-big-000" },
			{ PlanetEntity::VisualType::Azure,				"Azure-big-000" },
			{ PlanetEntity::VisualType::Bluegas,			"Bluegas-big-000" },
			{ PlanetEntity::VisualType::Carbide,			"Carbide-big-000" },
			{ PlanetEntity::VisualType::Chlorine,			"Chlorine-big-000" },
			{ PlanetEntity::VisualType::Chondrite,			"Chondrite-big-000" },
			{ PlanetEntity::VisualType::Cimmerian,			"Cimmerian-big-000" },
			{ PlanetEntity::VisualType::Copper,				"Copper-big-000" },
			{ PlanetEntity::VisualType::Crimson,			"Crimson-big-000" },
			{ PlanetEntity::VisualType::Cyangas,			"Cyangas-big-000" },
			{ PlanetEntity::VisualType::Cyanic,				"Cyanic-big-000" },
			{ PlanetEntity::VisualType::Dust,				"Dust-big-000" },
			{ PlanetEntity::VisualType::Emerald,			"Emerald-big-000" },
			{ PlanetEntity::VisualType::Fluorescent,		"Fluorescent-big-000" },
			{ PlanetEntity::VisualType::Green,				"Green-big-000" },
			{ PlanetEntity::VisualType::Greengas,			"Greengas-big-000" },
			{ PlanetEntity::VisualType::Greygas,			"Greygas-big-000" },
			{ PlanetEntity::VisualType::Halide,				"Halide-big-000" },
			{ PlanetEntity::VisualType::Hydrocarbon,		"Hydrocarbon-big-000" },
			{ PlanetEntity::VisualType::Infrared,			"Infrared-big-000" },
			{ PlanetEntity::VisualType::Iodine,				"Iodine-big-000" },
			{ PlanetEntity::VisualType::Lanthanide,			"Lanthanide-big-000" },
			{ PlanetEntity::VisualType::Magma,				"Magma-big-000" },
			{ PlanetEntity::VisualType::Magnetic,			"Magnetic-big-000" },
			{ PlanetEntity::VisualType::Maroon,				"Maroon-big-000" },
			{ PlanetEntity::VisualType::Metal,				"Metal-big-000" },
			{ PlanetEntity::VisualType::Noble,				"Noble-big-000" },
			{ PlanetEntity::VisualType::Oolite,				"Oolite-big-000" },
			{ PlanetEntity::VisualType::Opalescent,			"Opalescent-big-000" },
			{ PlanetEntity::VisualType::Organic,			"Organic-big-000" },
			{ PlanetEntity::VisualType::Pellucid,			"Pellucid-big-000" },
			{ PlanetEntity::VisualType::Plutonic,			"Plutonic-big-000" },
			{ PlanetEntity::VisualType::Primordial,			"Primordial-big-000" },
			{ PlanetEntity::VisualType::Purple,				"Purple-big-000" },
			{ PlanetEntity::VisualType::Purplegas,			"Purplegas-big-000" },
			{ PlanetEntity::VisualType::Quasidegenerate,	"Quasidegenerate-big-000" },
			{ PlanetEntity::VisualType::Radioactive,		"Radioactive-big-000" },
			{ PlanetEntity::VisualType::Rainbow,			"Rainbow-big-000" },
			{ PlanetEntity::VisualType::Redgas,				"Redgas-big-000" },
			{ PlanetEntity::VisualType::Redux,				"Redux-big-000" },
			{ PlanetEntity::VisualType::Ruby,				"Ruby-big-000" },
			{ PlanetEntity::VisualType::Sapphire,			"Sapphire-big-000" },
			{ PlanetEntity::VisualType::Selenic,			"Selenic-big-000" },
			{ PlanetEntity::VisualType::Shattered,			"Shattered-big-000" },
			{ PlanetEntity::VisualType::Slaveshield,		"Slaveshield-big-000" },
			{ PlanetEntity::VisualType::Superdense,			"Superdense-big-000" },
			{ PlanetEntity::VisualType::Telluric,			"Telluric-big-000" },
			{ PlanetEntity::VisualType::Treasure,			"Treasure-big-000" },
			{ PlanetEntity::VisualType::Ultramarine,		"Ultramarine-big-000" },
			{ PlanetEntity::VisualType::Urea,				"Urea-big-000" },
			{ PlanetEntity::VisualType::Vinylogous,			"Vinylogous-big-000" },
			{ PlanetEntity::VisualType::Violetgas,			"Violetgas-big-000" },
			{ PlanetEntity::VisualType::Water,				"Water-big-000" },
			{ PlanetEntity::VisualType::Xenolithic,			"Xenolithic-big-000" },
			{ PlanetEntity::VisualType::Yellowgas,			"Yellowgas-big-000" },
			{ PlanetEntity::VisualType::Yttric,				"Yttric-big-000" },
        };
}

SFMLPlanetEntityRenderer::SFMLPlanetEntityRenderer(PlanetEntity& entity)
    : m_entity(entity)
    , m_planetImage(SFMLAssetLoader::Instance().getTexture(kPlanetAssetNames.at(entity.visualType())))
{
    m_sprite.setTexture(*m_planetImage->texture);
    m_sprite.setTextureRect(m_planetImage->region);
    m_sprite.setOrigin(sf::Vector2f{ m_planetImage->region.width / 2.0f, m_planetImage->region.height / 2.0f });
}

void SFMLPlanetEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto planetRadius = m_entity.radius();
    const auto planetPos = m_entity.position();

    const auto scaleFactor = planetRadius * 2 / std::max(m_planetImage->region.width, m_planetImage->region.height);

    m_sprite.setScale(sf::Vector2f{ scaleFactor, scaleFactor });
    m_sprite.setPosition(ToSFMLVector(planetPos));

    renderer.draw(m_sprite);
}
