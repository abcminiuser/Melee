#pragma once

#include "EngineTypes.hpp"

#include <memory>

namespace Melee
{
    class RenderContext;

    class Entity : public std::enable_shared_from_this<Entity>
    {
    public:
        enum class Type
        {
            Asteroid,
            Exhaust,
            Explosion,
            Planet,
            Player,
            Projectile,
        };

        struct Properties
        {
            float       mass_kg = 0;
            float       radius_km = 0;
            uint32_t    maxVelocity = 0;
            bool        collidable = true;
        };

        struct PreCollisionState
        {
        	Vector2d	heading;
        	Vector2d	position;
        	Vector2d	velocity;
        };

    public:
        explicit                        Entity(Type type, const Properties& properties, const Point& pos, const Point& vel = {}, const Point& acc = {});
        virtual                         ~Entity() = default;

        virtual void                    update(Engine& engine, uint32_t msElapsed);
        virtual void                    collide(Engine& engine, const Entity& otherEntity, const PreCollisionState& otherEntityState);

        Type                            type() const				{ return m_type; }
        virtual const Properties&    	properties() const			{ return m_properties; }

        Point                           position() const			{ return m_position; }
        Vector2d                        heading() const				{ return m_heading; }
        Vector2d                        velocity() const			{ return m_velocity; }

		PreCollisionState 				preCollisionState() const	{ return { m_heading, m_position, m_velocity }; }

        std::shared_ptr<RenderContext>& rendererContext()			{ return m_rendererContext;  }

        void                            moveTo(const Vector2d& newPosition);
        void                            applyExternalForce(const Vector2d& forceVector);

    protected:
        const Type                      m_type;

        Properties                      m_properties;

        uint32_t                        m_maxVelocitySquared = 0;

        Point                           m_position;
        Vector2d                        m_heading = { 1, 0 };

        Vector2d                        m_velocity;
        Vector2d                        m_acceleration;

        Vector2d                        m_externalAcceleration;

        std::shared_ptr<RenderContext>  m_rendererContext;
    };
}
