#pragma once

#include "Util/VectorMath.hpp"
#include "Util/MatrixMath.hpp"

#include <list>
#include <memory>

namespace Melee
{
    class Engine;
    class Entity;

    using Point = Vector2d;

    struct Rectangle
    {
        Vector2d origin;
        Vector2d size;

        inline void inflate(const Vector2d& v)
        {
            inflate(v.x, v.y);
        }

        inline void inflate(float width, float height)
        {
            origin.x -= width / 2;
            origin.y -= height / 2;

            size.x += width;
            size.y += height;
        }
    };

    using EntityList = std::list<std::shared_ptr<Entity>>;
}
