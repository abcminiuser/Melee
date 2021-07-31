#pragma once

#include <SFML/Audio.hpp>

#include "Engine/Engine.hpp"

#include <array>

namespace Melee
{
    class Engine;

    class SFMLAudio
        : public Engine::Observer
    {
        static inline constexpr auto kMaxSimultaneousSounds = 32;

    public:
        explicit        SFMLAudio(Engine& engine);
        virtual         ~SFMLAudio();

        void            setVolume(float percent);

    // Engine::Observer i.f:
    public:
        void            entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity) override;
        void            collision(Engine& engine, const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2) override;

    private:
        void            playSoundEffect(const sf::SoundBuffer& sound);

    private:
        using SoundList = std::array<sf::Sound, kMaxSimultaneousSounds>;

        Engine&         m_engine;

        sf::Music       m_music;

        sf::SoundBuffer m_explosionSound;
        sf::SoundBuffer m_projectileSound;
        sf::SoundBuffer m_collisionSound;

        SoundList       m_sounds;
    };

    using Audio = SFMLAudio;
}
