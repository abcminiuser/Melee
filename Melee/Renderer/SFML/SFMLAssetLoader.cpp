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

        const auto textureSize = texture->getSize();

        std::ifstream metadata((asset.path().parent_path() / asset.path().stem()).string() + ".dat");
        if (metadata.is_open())
        {
            std::string line;
            while (std::getline(metadata, line))
            {
                size_t currPos = 0;
                size_t nextPos = 0;

                const auto NextField = [&]()
                    {
                        nextPos = line.find(',', currPos);
                        const auto fieldValue = line.substr(currPos, nextPos - currPos);
                        currPos = nextPos + 1;

                        return fieldValue;
                    };

                std::string assetName = NextField();

                CachedTexture cachedEntry = {};
                cachedEntry.texture = texture;

                for (auto* coordinate : { &cachedEntry.region.left, &cachedEntry.region.top, &cachedEntry.region.width, &cachedEntry.region.height })
                    *coordinate = std::stoi(NextField());

                m_textureCache.emplace(ToCacheKey(assetName), cachedEntry);
            }
        }
        else
        {
            auto assetName = asset.path().filename().stem().string();

            CachedTexture cachedEntry = {};
            cachedEntry.texture = texture;
            cachedEntry.region = { 0, 0, (int)textureSize.x, (int)textureSize.y };

            m_textureCache.emplace(ToCacheKey(assetName), cachedEntry);
        }
    }

    for (const auto& asset : std::filesystem::directory_iterator(kAssetBasePath / "Fonts"))
    {
        if (asset.path().extension() != ".ttf")
            continue;

        auto assetName = asset.path().filename().stem().string();

        auto font = std::make_shared<sf::Font>();
        font->loadFromFile(asset.path().string());

        CachedFont cachedFont;
        cachedFont = font;

        m_fontCache.emplace(ToCacheKey(assetName), cachedFont);
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
