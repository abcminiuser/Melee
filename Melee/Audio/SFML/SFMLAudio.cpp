#include "SFMLAudio.hpp"

#include <map>

using namespace Melee;

namespace
{
    const std::map<WeaponEntity::VisualType, std::string> kWeaponEffectAssetNames
    {
        { WeaponEntity::VisualType::Laser, "laser-shot-3" },
    };
}

SFMLAudio::SFMLAudio(Engine& engine)
    : m_engine(engine)
    , m_music(SFMLAudioAssetLoader::Instance().getMusic("and-the-machines-came-at-midnight"))
{
    m_engine.addObserver(this);

    m_music->music.setLoop(true);
    m_music->music.play();
}

SFMLAudio::~SFMLAudio()
{
    m_engine.removeObserver(this);
}

void SFMLAudio::setMasterVolume(float percent)
{
    percent = std::clamp(percent, 0.0f, 100.0f);

    sf::Listener::setGlobalVolume(percent);
}

void SFMLAudio::setMusicVolume(float percent)
{
    percent = std::clamp(percent, 0.0f, 100.0f);

    m_music->music.setVolume(percent);
}

void SFMLAudio::entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity)
{
    switch (entity->type())
    {
        case Entity::Type::Explosion:
        {
            // Only the initial explosion entity will be parentless, any sub-explosions are owned by the initial explosion.
            if (!entity->parentEntity())
                playSoundEffect("explosion1", entity->position());

            break;
        }

        case Entity::Type::Weapon:
        {
            const auto weaponEntity = std::dynamic_pointer_cast<WeaponEntity>(entity);

            if (auto soundEffect = kWeaponEffectAssetNames.find(weaponEntity->visualType()); soundEffect != kWeaponEffectAssetNames.end())
                playSoundEffect(soundEffect->second, entity->position());

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

    if (AreEntitiesOfType(Entity::Type::Ship, Entity::Type::Ship))
        playSoundEffect("robot-footstep_1", entity1->position());
    else if (AreEntitiesOfType(Entity::Type::Ship, Entity::Type::Planet))
        playSoundEffect("robot-footstep_1", entity1->position());
    else if (AreEntitiesOfType(Entity::Type::Ship, Entity::Type::Asteroid))
        playSoundEffect("robot-footstep_1", entity1->position());
}

void SFMLAudio::playSoundEffect(const std::string& name, Point position)
{
    const auto freeSoundEffect = std::find_if(m_sounds.begin(), m_sounds.end(), [](const auto& s) { return s.getStatus() == sf::Sound::Stopped; });
    if (freeSoundEffect == m_sounds.end())
        return;

    auto soundEffect = SFMLAudioAssetLoader::Instance().getSoundEffect(name);

    freeSoundEffect->setBuffer(soundEffect->soundEffect);
    freeSoundEffect->setPosition(position.x, position.y, 0);
    freeSoundEffect->play();
}
