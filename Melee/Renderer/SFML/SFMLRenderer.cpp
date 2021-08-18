#include "SFMLRenderer.hpp"
#include "SFMLRenderable.hpp"
#include "SFMLUtils.hpp"

#include "Entities/SFMLAsteroidEntityRenderer.hpp"
#include "Entities/SFMLExhaustEntityRenderer.hpp"
#include "Entities/SFMLExplosionEntityRenderer.hpp"
#include "Entities/SFMLPlanetEntityRenderer.hpp"
#include "Entities/SFMLShipEntityRenderer.hpp"
#include "Entities/SFMLWeaponEntityRenderer.hpp"

#include "UI/SFMLShipHudTileRenderer.hpp"

#include "Engine/Engine.hpp"

#include <unordered_map>

using namespace Melee;

namespace
{
    using KeyMap = std::unordered_map<sf::Keyboard::Key, ShipEntity::KeyEvent>;

    // FIXME: User key mappings
    const KeyMap kPlayer1Keys =
    {
        { sf::Keyboard::Key::W,          ShipEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::A,          ShipEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::S,          ShipEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::D,          ShipEntity::KeyEvent::RotateRight },
        { sf::Keyboard::Key::Tilde,      ShipEntity::KeyEvent::FirePrimary },
        { sf::Keyboard::Key::Tab,        ShipEntity::KeyEvent::FireSpecial },
    };
    const KeyMap kPlayer2Keys =
    {
        { sf::Keyboard::Key::Up,         ShipEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::Left,       ShipEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::Down,       ShipEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::Right,      ShipEntity::KeyEvent::RotateRight },
        { sf::Keyboard::Key::RControl,   ShipEntity::KeyEvent::FirePrimary },
        { sf::Keyboard::Key::RShift,     ShipEntity::KeyEvent::FireSpecial },
    };

    constexpr auto kTargetFPS = 60;

    constexpr auto kViewportPadding_km = 10000.0f;
    constexpr auto kMinViewportAxisSize_km = 50000.0f;

    constexpr auto kWindowHeight = 1000;
    constexpr auto kWindowWidthMultiplierForHud = 1.2f;
}

SFMLRenderer::SFMLRenderer(Engine& engine)
    : m_engine(engine)
    , m_backgroundTexture(SFMLAssetLoader::Instance().getTexture("background"))
{
    m_engine.addObserver(this);

    const auto playfieldSize = m_engine.getPlayfieldSize();

    m_backgroundSprite.setTexture(*m_backgroundTexture->texture);
    m_backgroundSprite.setTextureRect(m_backgroundTexture->region);
    m_backgroundSprite.setScale(sf::Vector2f{ playfieldSize / m_backgroundTexture->region.width, playfieldSize / m_backgroundTexture->region.height });
}

SFMLRenderer::~SFMLRenderer()
{
    m_engine.removeObserver(this);
}

int SFMLRenderer::runModal()
{
    constexpr auto kWindowWidth = static_cast<int>(kWindowHeight * kWindowWidthMultiplierForHud);

    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight), "Melee");

    constexpr auto kMillisecondsPerFrame = 1000 / kTargetFPS;
    constexpr auto kWidthForPlayfield = 1.0f / kWindowWidthMultiplierForHud;

    m_playfieldView.setViewport(sf::FloatRect(0, 0, kWidthForPlayfield, 1));
    m_playerHudView.setViewport(sf::FloatRect(kWidthForPlayfield, 0, 1 - kWidthForPlayfield, 1));

    m_playerHudView.setSize(250, 1000);
    m_playerHudView.setCenter(250 / 2, 1000 / 2);

    window.setFramerateLimit(kTargetFPS);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        processEvents(window);

        m_engine.update(kMillisecondsPerFrame);

        updatePlayfieldViewport();

        window.clear();

        window.setView(m_playfieldView);
        renderPlayfield(window);

        window.setView(m_playerHudView);
        renderPlayerHud(window);

        window.display();
    }

    return 0;
}

void SFMLRenderer::updatePlayfieldViewport()
{
    const auto playfieldSize = m_engine.getPlayfieldSize();

    auto playfieldView = m_engine.getShipsBoundingBox();

    // We want some padding around the players, so they aren't at the extreme edges of the resuling view unless unavoidable.
    playfieldView.inflate(Vector2d{ kViewportPadding_km, kViewportPadding_km });

    // We want a rectangular viewport that's at least as large as the minimum size, but the larger of the two bounding box axis.
    const auto currentViewSize = std::max(playfieldView.size.x, playfieldView.size.y);
    const auto newViewSize = std::clamp<float>(currentViewSize, kMinViewportAxisSize_km, playfieldSize);
    playfieldView.origin.x -= (newViewSize - playfieldView.size.x) / 2;
    playfieldView.origin.y -= (newViewSize - playfieldView.size.y) / 2;
    playfieldView.size = { newViewSize, newViewSize };

    // Don't allow the camera to go outside the playfield area
    playfieldView.origin.x = std::clamp<float>(playfieldView.origin.x, 0, playfieldSize - playfieldView.size.x);
    playfieldView.origin.y = std::clamp<float>(playfieldView.origin.y, 0, playfieldSize - playfieldView.size.y);

    // Update the playfield view with the new calculated viewport
    m_playfieldView.setCenter(ToSFMLVector(playfieldView.center()));
    m_playfieldView.setSize(ToSFMLVector(playfieldView.size));
}

void SFMLRenderer::processEvents(sf::RenderWindow& window)
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

            default:
                break;
        }
    }
}

void SFMLRenderer::handleKey(sf::Keyboard::Key key, bool down)
{
    if (m_shipEntities.size() >= 1)
    {
        if (kPlayer1Keys.count(key))
            m_shipEntities[0]->handleKey(kPlayer1Keys.at(key), down);
    }

    if (m_shipEntities.size() >= 2)
    {
        if (kPlayer2Keys.count(key))
            m_shipEntities[1]->handleKey(kPlayer2Keys.at(key), down);
    }
}

void SFMLRenderer::renderPlayfield(sf::RenderTarget& target)
{
    target.draw(m_backgroundSprite);

    for (const auto& entity : m_engine.getEntities())
    {
        const auto& rendererContext = getEntityRenderContext(entity);

        if (rendererContext->playfieldRenderer)
            rendererContext->playfieldRenderer->render(target);
    }
}

void SFMLRenderer::renderPlayerHud(sf::RenderTarget& target)
{
    sf::RectangleShape r;
    r.setFillColor(sf::Color(100, 100, 100));
    r.setSize(sf::Vector2f{ 250, 1000 });
    target.draw(r);

    for (const auto& entity : m_shipEntities)
    {
        const auto& rendererContext = getEntityRenderContext(entity);

        if (rendererContext->uiRenderer)
            rendererContext->uiRenderer->render(target);
    }
}

std::shared_ptr<RenderContext> SFMLRenderer::getEntityRenderContext(const std::shared_ptr<Entity>& entity)
{
    auto& rendererContext = entity->rendererContext();
    if (rendererContext)
        return rendererContext;

    rendererContext = std::make_shared<RenderContext>();

    switch (entity->type())
    {
        case Entity::Type::Asteroid:
        {
            const auto asteroidEntity = std::dynamic_pointer_cast<AsteroidEntity>(entity);

            rendererContext->playfieldRenderer = std::make_unique<SFMLAsteroidEntityRenderer>(*asteroidEntity);
            break;
        }

        case Entity::Type::Exhaust:
        {
            const auto exhaustEntity = std::dynamic_pointer_cast<ExhaustEntity>(entity);

            rendererContext->playfieldRenderer = std::make_unique<SFMLExhaustEntityRenderer>(*exhaustEntity);
            break;
        }

        case Entity::Type::Explosion:
        {
            const auto explosionEntity = std::dynamic_pointer_cast<ExplosionEntity>(entity);

            rendererContext->playfieldRenderer = std::make_unique<SFMLExplosionEntityRenderer>(*explosionEntity);
            break;
        }

        case Entity::Type::Planet:
        {
            const auto planetEntity = std::dynamic_pointer_cast<PlanetEntity>(entity);

            rendererContext->playfieldRenderer = std::make_unique<SFMLPlanetEntityRenderer>(*planetEntity);
            break;
        }

        case Entity::Type::Ship:
        {
            const auto shipEntity = std::dynamic_pointer_cast<ShipEntity>(entity);
            const auto shipIndex = m_shipEntities.size();

            m_shipEntities.push_back(shipEntity);

            sf::FloatRect tileArea(0, 1000 * shipIndex / 4.0f, 250, 1000 / 4.0f);

            rendererContext->playfieldRenderer = std::make_unique<SFMLShipEntityRenderer>(*shipEntity);
            rendererContext->uiRenderer = std::make_unique<SFMLShipHudTileRenderer>(*shipEntity, tileArea);
            break;
        }

        case Entity::Type::Weapon:
        {
            const auto projectileEntity = std::dynamic_pointer_cast<WeaponEntity>(entity);

            rendererContext->playfieldRenderer = std::make_unique<SFMLWeaponEntityRenderer>(*projectileEntity);
            break;
        }
    }

    return rendererContext;
}
