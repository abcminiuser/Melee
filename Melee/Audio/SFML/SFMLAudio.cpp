#include "SFMLAudio.hpp"

using namespace Melee;

SFMLAudio::SFMLAudio(Engine& engine)
    : m_engine(engine)
{
    m_engine.addObserver(this);

    m_explosionSound.loadFromFile("Assets/Audio/Explosion1.ogg");
    m_projectileSound.loadFromFile("Assets/Audio/Laser-Shot-3.ogg");
    m_collisionSound.loadFromFile("Assets/Audio/Robot-Footstep_1.ogg");

    m_music.openFromFile("Assets/Audio/And-the-Machines-Came-at-Midnight.ogg");
    m_music.setLoop(true);
    m_music.play();
}

SFMLAudio::~SFMLAudio()
{
    m_engine.removeObserver(this);
}

void SFMLAudio::setVolume(float percent)
{
    percent = std::clamp(percent, 0.0f, 100.0f);

    m_music.setVolume(percent);

    for (auto& soundEffect : m_sounds)
        soundEffect.setVolume(percent);
}

void SFMLAudio::entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    switch (entity->type())
    {
        case Entity::Type::Explosion:
        {
            playSoundEffect(m_explosionSound, entity->position());
            break;
        }

        case Entity::Type::Projectile:
        {
            playSoundEffect(m_projectileSound, entity->position());
            break;
        }

        default:
            break;
    }
}

void SFMLAudio::collision(Engine& engine, const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2)
{
    const auto AreEntitiesOfType =
        [e1Type = entity1->type(), e2Type = entity2->type()](Entity::Type type1, Entity::Type type2)
        {
            return (e1Type == type1 && e2Type == type2) || (e1Type == type2 && e2Type == type1);
        };

    if (AreEntitiesOfType(Entity::Type::Player, Entity::Type::Player))
        playSoundEffect(m_collisionSound, entity1->position());
    else if (AreEntitiesOfType(Entity::Type::Player, Entity::Type::Planet))
        playSoundEffect(m_collisionSound, entity1->position());
    else if (AreEntitiesOfType(Entity::Type::Player, Entity::Type::Asteroid))
        playSoundEffect(m_collisionSound, entity1->position());
}

void SFMLAudio::playSoundEffect(const sf::SoundBuffer& sound, Point position)
{
    const auto freeSoundEffect = std::find_if(m_sounds.begin(), m_sounds.end(), [](const auto& s) { return s.getStatus() == sf::Sound::Stopped; });
    if (freeSoundEffect == m_sounds.end())
        return;

    freeSoundEffect->setBuffer(sound);
    freeSoundEffect->setPosition(position.x, position.y, 0);
    freeSoundEffect->setMinDistance(std::max(1.0f, m_engine.getPlayersBoundingBox().size.lengthSquared()));
    freeSoundEffect->play();
}
