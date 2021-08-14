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

		auto cachedEntry = std::make_shared<SoundEffect>();

		cachedEntry->soundEffect.loadFromFile(asset.path().string());

		m_soundEffectCache.emplace(ToCacheKey(assetName), std::move(cachedEntry));
	}

	for (const auto& asset : std::filesystem::directory_iterator(kAssetBasePath / "Music"))
	{
		if (asset.path().extension() != ".ogg")
			continue;

		auto assetName = asset.path().filename().stem().string();

		auto cachedEntry = std::make_shared<Music>();

		cachedEntry->music.openFromFile(asset.path().string());

		m_musicCache.emplace(ToCacheKey(assetName), std::move(cachedEntry));
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
