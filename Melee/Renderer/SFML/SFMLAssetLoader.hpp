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
        struct Texture
        {
            std::shared_ptr<sf::Texture>    texture;
            sf::IntRect                     region;
        };

        struct Font
        {
            sf::Font            font;
        };

        using CachedTexture = std::shared_ptr<Texture>;
        using CachedFont = std::shared_ptr<Font>;

        CachedTexture           getTexture(const std::string& name) const;
        CachedFont              getFont(const std::string& name) const;

    private:
        template <typename T>
        using Cache = std::unordered_map<std::string, T>;

        Cache<CachedTexture>    m_textureCache;
        Cache<CachedFont>       m_fontCache;
    };
}
