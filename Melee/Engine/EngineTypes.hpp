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

    using EntityList = std::list<std::shared_ptr<Entity>>;
}
