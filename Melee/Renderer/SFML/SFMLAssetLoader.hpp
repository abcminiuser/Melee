#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace Melee
{
    class SFMLAssetLoader final
    {
    public:
        static auto& Instance()
        {
            static SFMLAssetLoader s_instance;
            return s_instance;
        }

    private:
        explicit                SFMLAssetLoader();
                                ~SFMLAssetLoader() = default;

    public:
        struct CachedTexture
        {
            std::shared_ptr<sf::Texture>    texture;
            sf::IntRect                     region;
        };

        using CachedFont = std::shared_ptr<sf::Font>;

        CachedTexture           getTexture(const std::string& name) const;
        CachedFont              getFont(const std::string& name) const;

    private:
        template <typename T>
        using Cache = std::unordered_map<std::string, T>;

        Cache<CachedTexture>    m_textureCache;
        Cache<CachedFont>       m_fontCache;
    };
}
