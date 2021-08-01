#include "SFMLAudio.hpp"

using namespace Melee;

SFMLAudio::SFMLAudio(Engine& engine)
    : m_engine(engine)
    , m_music(SFMLAudioAssetLoader::Instance().getMusic("and-the-machines-came-at-midnight"))
{
    m_engine.addObserver(this);

    m_music->setLoop(true);
    m_music->play();
}

SFMLAudio::~SFMLAudio()
{
    m_engine.removeObserver(this);
}

void SFMLAudio::setVolume(float percent)
{
    percent = std::clamp(percent, 0.0f, 100.0f);

    m_music->setVolume(percent);

    for (auto& soundEffect : m_sounds)
        soundEffect.setVolume(percent);
}

void SFMLAudio::entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    switch (entity->type())
    {
        case Entity::Type::Explosion:
        {
            playSoundEffect("explosion1", entity->position());
            break;
        }

        case Entity::Type::Projectile:
        {
            playSoundEffect("laser-shot-3", entity->position());
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
        playSoundEffect("robot-footstep_1", entity1->position());
    else if (AreEntitiesOfType(Entity::Type::Player, Entity::Type::Planet))
        playSoundEffect("robot-footstep_1", entity1->position());
    else if (AreEntitiesOfType(Entity::Type::Player, Entity::Type::Asteroid))
        playSoundEffect("robot-footstep_1", entity1->position());
}

void SFMLAudio::playSoundEffect(const std::string& name, Point position)
{
    const auto freeSoundEffect = std::find_if(m_sounds.begin(), m_sounds.end(), [](const auto& s) { return s.getStatus() == sf::Sound::Stopped; });
    if (freeSoundEffect == m_sounds.end())
        return;

    freeSoundEffect->setBuffer(*SFMLAudioAssetLoader::Instance().getSoundEffect(name));
    freeSoundEffect->setPosition(position.x, position.y, 0);
    freeSoundEffect->setMinDistance(std::max(1.0f, m_engine.getPlayersBoundingBox().size.length()));
    freeSoundEffect->play();
}
