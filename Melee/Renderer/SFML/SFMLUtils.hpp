#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/EngineTypes.hpp"

#include <cmath>
#include <algorithm>

namespace Melee
{
	static inline const sf::Color HSVColor(float H, float S, float V)
    {
        if (std::clamp<float>(H, 0, 360) != H || std::clamp<float>(S, 0, 100) != S || std::clamp<float>(V, 0, 100) != V)
            throw std::out_of_range("Invalid range");

        float s = S / 100;
        float v = V / 100;
        float C = s * v;
        float X = C * (1 - std::fabs(std::fmod(H / 60.0, 2) - 1));
        float m = v - C;

        uint8_t c = static_cast<uint8_t>((C + m) * 255);
        uint8_t x = static_cast<uint8_t>((X + m) * 255);

        if (H >= 0 && H < 60)
            return { c, x, 0 };
        else if (H >= 60 && H < 120)
            return { x, c, 0 };
        else if (H >= 120 && H < 180)
            return { 0, c, x };
        else if (H >= 180 && H < 240)
            return { 0, x, c };
        else if (H >= 240 && H < 300)
            return { x, 0, c };
        else
            return { c, 0, x };
    }

	static inline sf::Vector2f ToSFMLVector(const Vector2d& v)
    {
        return sf::Vector2f{ v.x, v.y };
    }

    static inline float ToDegrees(const Vector2d& v)
    {
        return 90 + (180 * std::atan2(v.y, v.x) / static_cast<float>(M_PI));
    }
}
