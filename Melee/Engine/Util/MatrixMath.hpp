#pragma once

#include <cstdint>
#include <cmath>

namespace Melee
{
    struct Matrix2x2 final
    {
        float x0y0 = 0;
        float x1y0 = 0;
        float x0y1 = 0;
        float x1y1 = 0;

        Vector2d operator *(const Vector2d& v) const
        {
            return Vector2d{ v.x * x0y0 + v.y * x1y0, v.x * x0y1 + v.y * x1y1 };
        }
    };

    inline Matrix2x2 RotationMatrix(float degrees)
    {
        const float rads = degrees * static_cast<float>(2 * M_PI / 360);
        const float cosAng = std::cosf(rads);
        const float sinAng = std::sinf(rads);

        return Matrix2x2{ cosAng, -sinAng, sinAng, cosAng };
    }
}
