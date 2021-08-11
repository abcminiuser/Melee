#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class ExhaustEntity : public Entity
    {
    public:
        struct ExhaustProperties : public Properties
        {
            ExhaustProperties()
            {
                collidable = false;
                wrappable = false;
            }

            uint32_t maxAge_ms = 1000;
        };

    public:
        explicit                    ExhaustEntity(const std::shared_ptr<Entity>& parent, const ExhaustProperties& properties, const Point& position, const Vector2d& velocity);
        virtual                     ~ExhaustEntity() = default;

        auto                        maxAge() const      { return m_maxAge_ms; }

        uint32_t                    age() const         { return m_age; }

    // Entity i/f:
    public:
        void                        update(Engine& engine, uint32_t msElapsed) override;

    private:
        uint32_t                    m_maxAge_ms = 0;

        uint32_t                    m_age = 0;
    };
}
