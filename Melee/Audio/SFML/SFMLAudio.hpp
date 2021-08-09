#pragma once

#include <SFML/Audio.hpp>

#include "Engine/Engine.hpp"

#include "SFMLAudioAssetLoader.hpp"

#include <array>

namespace Melee
{
    class SFMLAudio
        : public Engine::Observer
    {
        static inline constexpr auto kMaxSimultaneousSounds = 32;

    public:
        explicit        SFMLAudio(Engine& engine);
        virtual         ~SFMLAudio();

        void            setMasterVolume(float percent);
        void            setMusicVolume(float percent);

    // Engine::Observer i.f:
    public:
        void            entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity) override;
        void            collision(Engine& engine, const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2) override;

    private:
        float           attenuateFromPlayerBoundingBox(Engine& engine, const std::shared_ptr<Entity>& entity) const;
        void            playSoundEffect(const std::string& name, Point position);

    private:
        using SoundList = std::array<sf::Sound, kMaxSimultaneousSounds>;

        Engine&                             m_engine;

        SFMLAudioAssetLoader::CachedMusic   m_music;

        SoundList                           m_sounds;
    };
}
