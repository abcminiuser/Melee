#include "SFMLAssetLoader.hpp"

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

SFMLAssetLoader::SFMLAssetLoader()
{
    for (const auto& asset : std::filesystem::directory_iterator(kAssetBasePath / "Images"))
    {
        if (asset.path().extension() != ".png")
            continue;

        auto texture = std::make_shared<sf::Texture>();
        texture->loadFromFile(asset.path().string());

        if (std::ifstream metadata(asset.path().parent_path() / (asset.path().stem().string() + ".dat")); metadata.is_open())
        {
            std::string line;
            while (std::getline(metadata, line))
            {
                size_t currPos = 0;
                size_t nextPos = 0;

                const auto NextField = [&]()
                    {
                        std::string fieldValue;

                        nextPos = line.find(',', currPos);
                        fieldValue = line.substr(currPos, nextPos - currPos);
                        currPos = nextPos + 1;

                        return fieldValue;
                    };

                const auto assetName = NextField();

                auto cachedEntry = std::make_shared<Texture>();

                cachedEntry->texture = texture;

                for (auto* coordinate : { &cachedEntry->region.left, &cachedEntry->region.top, &cachedEntry->region.width, &cachedEntry->region.height })
                    *coordinate = std::stoi(NextField());

                m_textureCache.emplace(ToCacheKey(assetName), std::move(cachedEntry));
            }
        }
        else
        {
            const auto assetName = asset.path().filename().stem().string();

            auto cachedEntry = std::make_shared<Texture>();

            cachedEntry->texture = texture;

            const auto textureSize = cachedEntry->texture->getSize();

            cachedEntry->region = { 0, 0, static_cast<int>(textureSize.x), static_cast<int>(textureSize.y) };

            m_textureCache.emplace(ToCacheKey(assetName), std::move(cachedEntry));
        }
    }

    for (const auto& asset : std::filesystem::directory_iterator(kAssetBasePath / "Fonts"))
    {
        if (asset.path().extension() != ".ttf")
            continue;

        const auto assetName = asset.path().filename().stem().string();

        auto cachedEntry = std::make_shared<Font>();
        cachedEntry->font.loadFromFile(asset.path().string());

        m_fontCache.emplace(ToCacheKey(assetName), std::move(cachedEntry));
    }
}

SFMLAssetLoader::CachedTexture SFMLAssetLoader::getTexture(const std::string& name) const
{
    const auto foundEntry = m_textureCache.find(ToCacheKey(name));
    if (foundEntry == m_textureCache.end())
        throw std::runtime_error("Missing texture: " + name);

    return foundEntry->second;
}

SFMLAssetLoader::CachedFont SFMLAssetLoader::getFont(const std::string& name) const
{
    const auto foundEntry = m_fontCache.find(ToCacheKey(name));
    if (foundEntry == m_fontCache.end())
        throw std::runtime_error("Missing font: " + name);

    return foundEntry->second;
}
