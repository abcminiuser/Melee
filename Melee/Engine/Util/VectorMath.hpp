#pragma once

#include <cstdint>
#include <cmath>
#include <tuple>

namespace Melee
{
    struct Vector2d final
    {
        float x = 0;
        float y = 0;

        Vector2d normalised() const noexcept
        {
            return Vector2d{ x, y } / length();
        }

        float length() const noexcept
        {
            return sqrtf(x * x + y * y);
        }

        constexpr float lengthSquared() const noexcept
        {
            return x * x + y * y;
        }

        constexpr Vector2d dotProduct(const Vector2d& other) const noexcept
        {
            return Vector2d{ x * other.x, y * other.y };
        }

        constexpr bool operator==(const Vector2d& other) const noexcept
        {
            return std::tie(x, y) == std::tie(other.x, other.y);
        }

        constexpr bool operator!=(const Vector2d& other) const noexcept
        {
            return std::tie(x, y) != std::tie(other.x, other.y);
        }

        constexpr  Vector2d& operator +=(const Vector2d& other) noexcept
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        constexpr Vector2d& operator -=(const Vector2d& other) noexcept
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        constexpr Vector2d& operator *=(float scaler) noexcept
        {
            x *= scaler;
            y *= scaler;

            return *this;
        }

        constexpr Vector2d& operator /=(float scaler) noexcept
        {
            x /= scaler;
            y /= scaler;

            return *this;
        }

        constexpr Vector2d operator -() const noexcept
        {
            return Vector2d{ -x, -y };
        }

        constexpr Vector2d operator +(const Vector2d& other) const noexcept
        {
            Vector2d v = *this;
            v.operator+=(other);

            return v;
        }

        constexpr Vector2d operator -(const Vector2d& other) const noexcept
        {
            Vector2d v = *this;
            v.operator-=(other);

            return v;
        }

        constexpr Vector2d operator *(float scaler) const noexcept
        {
            Vector2d v = *this;
            v.operator*=(scaler);

            return v;
        }

        constexpr Vector2d operator /(float scaler) const noexcept
        {
            Vector2d v = *this;
            v.operator/=(scaler);

            return v;
        }
    };
}
