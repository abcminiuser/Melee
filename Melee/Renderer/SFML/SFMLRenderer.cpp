#include "SFMLRenderer.hpp"
#include "SFMLRenderable.hpp"
#include "SFMLUtils.hpp"

#include "Entities/SFMLAsteroidEntityRenderer.hpp"
#include "Entities/SFMLExhaustEntityRenderer.hpp"
#include "Entities/SFMLExplosionEntityRenderer.hpp"
#include "Entities/SFMLPlanetEntityRenderer.hpp"
#include "Entities/SFMLPlayerEntityRenderer.hpp"
#include "Entities/SFMLProjectileEntityRenderer.hpp"

#include "UI/SFMLPlayerHudTileRenderer.hpp"

#include "Engine/Engine.hpp"

#include <unordered_map>

using namespace Melee;

namespace
{
    using KeyMap = std::unordered_map<sf::Keyboard::Key, PlayerEntity::KeyEvent>;

    // FIXME: User key mappings
    const KeyMap kPlayer1Keys =
    {
        { sf::Keyboard::Key::W,          PlayerEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::A,          PlayerEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::S,          PlayerEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::D,          PlayerEntity::KeyEvent::RotateRight },
        { sf::Keyboard::Key::Tilde,      PlayerEntity::KeyEvent::FirePrimary },
        { sf::Keyboard::Key::Tab,        PlayerEntity::KeyEvent::FireSpecial },
    };
    const KeyMap kPlayer2Keys =
    {
        { sf::Keyboard::Key::Up,         PlayerEntity::KeyEvent::Thrust },
        { sf::Keyboard::Key::Left,       PlayerEntity::KeyEvent::RotateLeft },
        { sf::Keyboard::Key::Down,       PlayerEntity::KeyEvent::ReverseThrust },
        { sf::Keyboard::Key::Right,      PlayerEntity::KeyEvent::RotateRight },
        { sf::Keyboard::Key::RControl,   PlayerEntity::KeyEvent::FirePrimary },
        { sf::Keyboard::Key::RShift,     PlayerEntity::KeyEvent::FireSpecial },
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

    m_backgroundSprite.setTexture(*m_backgroundTexture.texture);
    m_backgroundSprite.setTextureRect(m_backgroundTexture.region);
    m_backgroundSprite.setScale(sf::Vector2f{ playfieldSize / m_backgroundTexture.region.width, playfieldSize / m_backgroundTexture.region.height });
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

    auto playfieldView = m_engine.getPlayersBoundingBox();

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
    for (const auto& entity : m_engine.getEntities())
    {
        if (entity->type() != Entity::Type::Player)
            continue;

        const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);
        const KeyMap* playerKeyMap = nullptr;

        if (playerEntity->index() == 0)
            playerKeyMap = &kPlayer1Keys;
        else if (playerEntity->index() == 1)
            playerKeyMap = &kPlayer2Keys;

        if (playerKeyMap && playerKeyMap->count(key))
            playerEntity->handleKey(playerKeyMap->at(key), down);
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

    for (const auto& entity : m_engine.getEntities())
    {
        if (entity->type() != Entity::Type::Player)
            continue;

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

        case Entity::Type::Player:
        {
            const auto playerEntity = std::dynamic_pointer_cast<PlayerEntity>(entity);

            sf::FloatRect tileArea(0, 1000 * playerEntity->index() / 4.0f, 250, 1000 / 4.0f);

            rendererContext->playfieldRenderer = std::make_unique<SFMLPlayerEntityRenderer>(*playerEntity);
            rendererContext->uiRenderer = std::make_unique<SFMLPlayerHudTileRenderer>(*playerEntity, tileArea);
            break;
        }

        case Entity::Type::Projectile:
        {
            const auto projectileEntity = std::dynamic_pointer_cast<ProjectileEntity>(entity);

            rendererContext->playfieldRenderer = std::make_unique<SFMLProjectileEntityRenderer>(*projectileEntity);
            break;
        }
    }

    return rendererContext;
}
