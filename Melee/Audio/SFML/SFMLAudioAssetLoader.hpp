#pragma once

#include <SFML/Audio.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace Melee
{
	class SFMLAudioAssetLoader final
	{
	public:
		static auto& Instance()
		{
			static SFMLAudioAssetLoader s_instance;
			return s_instance;
		}

	private:
		explicit					SFMLAudioAssetLoader();
									~SFMLAudioAssetLoader() = default;

	public:
		struct CachedSoundEffect
		{
			std::shared_ptr<sf::SoundBuffer> soundEffect;
		};

		struct CachedMusic
		{
			std::shared_ptr<sf::Music> music;
		};

		CachedSoundEffect			getSoundEffect(const std::string& name) const;
		CachedMusic					getMusic(const std::string& name) const;

	private:
		template <typename T>
		using Cache = std::unordered_map<std::string, T>;

		Cache<CachedSoundEffect>	m_soundEffectCache;
		Cache<CachedMusic>			m_musicCache;
	};
}
