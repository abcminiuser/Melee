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

        float lengthSquared() const
        {
            return x * x + y * y;
        }

        float length() const
        {
            return sqrtf(x * x + y * y);
        }

        Vector2d normalised() const
        {
            return Vector2d{ x, y } / length();
        }

        Vector2d dotProduct(const Vector2d& other) const
        {
            return Vector2d{ x * other.x, y * other.y };
        }

        bool operator==(const Vector2d& other) const
        {
            return std::tie(x, y) == std::tie(other.x, other.y);
        }

        bool operator!=(const Vector2d& other) const
        {
            return std::tie(x, y) != std::tie(other.x, other.y);
        }

        Vector2d& operator +=(const Vector2d& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        Vector2d& operator -=(const Vector2d& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        Vector2d& operator *=(float scaler)
        {
            x *= scaler;
            y *= scaler;

            return *this;
        }

        Vector2d& operator /=(float scaler)
        {
            x /= scaler;
            y /= scaler;

            return *this;
        }

        Vector2d operator -() const
        {
            return Vector2d{ -x, -y };
        }

        Vector2d operator +(const Vector2d& other) const
        {
            Vector2d v = *this;
            v.operator+=(other);

            return v;
        }

        Vector2d operator -(const Vector2d& other) const
        {
            Vector2d v = *this;
            v.operator-=(other);

            return v;
        }

        Vector2d operator *(float scaler) const
        {
            Vector2d v = *this;
            v.operator*=(scaler);

            return v;
        }

        Vector2d operator /(float scaler) const
        {
            Vector2d v = *this;
            v.operator/=(scaler);

            return v;
        }
    };
}
