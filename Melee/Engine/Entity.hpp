#pragma once

#include "EngineTypes.hpp"

#include <memory>

namespace Melee
{
    struct RenderContext;

    class Entity : public std::enable_shared_from_this<Entity>
    {
    public:
        enum class Type
        {
            Asteroid,
            Exhaust,
            Explosion,
            Planet,
            Ship,
            Weapon,
        };

        struct Properties
        {
            float       mass_kg = 0;
            float       radius_km = 0;
            float       maxVelocity_km_s = 0;
            bool        collidable = true;
            bool        wrappable = true;
            bool        targetable = true;
            bool        affectedByGravity = true;
            uint8_t     collisionDamage = 0;
        };

        struct PreCollisionState
        {
        	Vector2d	heading;
        	Vector2d	position;
        	Vector2d	velocity;
        };

    public:
        explicit                        Entity(Type type, const std::shared_ptr<Entity>& parent, const Properties& properties, const Point& pos);
        virtual                         ~Entity() = default;

        auto        					parentEntity() const 		{ return m_parentEntity; }

        virtual void                    update(Engine& engine, uint32_t msElapsed);
        virtual void                    collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState);

        Type                            type() const				{ return m_type; }

        auto                            mass() const                { return m_mass_kg; }
        auto                            radius() const              { return m_radius_km; }
        auto                            maxVelocity() const         { return m_maxVelocity_km_s; }
        auto                            collidable() const          { return m_collidable; }
        auto                            wrappable() const           { return m_wrappable; }
        auto                            targetable() const          { return m_targetable; }
        auto                            affectedByGravity() const   { return m_affectedByGravity; }
        auto                            collisionDamage() const     { return m_collisionDamage; }

        Point                           position() const			{ return m_position; }
        Vector2d                        heading() const				{ return m_heading; }
        Vector2d                        velocity() const			{ return m_velocity; }

		PreCollisionState 				preCollisionState() const	{ return { m_heading, m_position, m_velocity }; }

        std::shared_ptr<RenderContext>& rendererContext()			{ return m_rendererContext;  }

        void                            moveTo(const Vector2d& newPosition);
        void                            applyExternalForce(const Vector2d& forceVector);

    protected:
        const Type                      m_type;

        std::shared_ptr<Entity>			m_parentEntity;

        float                           m_mass_kg = 0;
        float                           m_radius_km = 0;
        float                           m_maxVelocity_km_s = 0;
        bool                            m_collidable = true;
        bool                            m_wrappable = true;
        bool                            m_targetable = true;
        bool                            m_affectedByGravity = true;
        uint8_t                         m_collisionDamage = 0;

        Point                           m_position;
        Vector2d                        m_heading = { 1, 0 };

        Vector2d                        m_velocity;
        Vector2d                        m_acceleration;

        Vector2d                        m_externalAcceleration;

        std::shared_ptr<RenderContext>  m_rendererContext;
    };
}
