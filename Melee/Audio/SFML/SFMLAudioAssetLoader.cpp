#include "SFMLAudioAssetLoader.hpp"

#include <filesystem>
#include <fstream>

using namespace Melee;

namespace
{
	const auto kAssetBasePath = std::filesystem::path("Assets");

	const std::string ToCacheKey(std::string name)
	{
		std::transform(name.begin(), name.end(), name.begin(), [](const auto c) { return std::tolower(c); });
		return name;
	}
}

SFMLAudioAssetLoader::SFMLAudioAssetLoader()
{
	for (const auto& asset : std::filesystem::directory_iterator(kAssetBasePath / "SoundEffects"))
	{
		if (asset.path().extension() != ".ogg")
			continue;

		auto assetName = asset.path().filename().stem().string();

		auto soundEffect = std::make_shared<sf::SoundBuffer>();
		soundEffect->loadFromFile(asset.path().string());

		CachedSoundEffect cachedSoundEffect;
		cachedSoundEffect = soundEffect;

		m_soundEffectCache.emplace(ToCacheKey(assetName), cachedSoundEffect);
	}

	for (const auto& asset : std::filesystem::directory_iterator(kAssetBasePath / "Music"))
	{
		if (asset.path().extension() != ".ogg")
			continue;

		auto assetName = asset.path().filename().stem().string();

		auto music = std::make_shared<sf::Music>();
		music->openFromFile(asset.path().string());

		CachedMusic cachedMusic;
		cachedMusic = music;

		m_musicCache.emplace(ToCacheKey(assetName), cachedMusic);
	}
}

SFMLAudioAssetLoader::CachedSoundEffect SFMLAudioAssetLoader::getSoundEffect(const std::string& name) const
{
	const auto foundEntry = m_soundEffectCache.find(ToCacheKey(name));
	if (foundEntry == m_soundEffectCache.end())
		throw std::runtime_error("Missing sound effect: " + name);

	return foundEntry->second;
}

SFMLAudioAssetLoader::CachedMusic SFMLAudioAssetLoader::getMusic(const std::string& name) const
{
	const auto foundEntry = m_musicCache.find(ToCacheKey(name));
	if (foundEntry == m_musicCache.end())
		throw std::runtime_error("Missing music: " + name);

	return foundEntry->second;
}
