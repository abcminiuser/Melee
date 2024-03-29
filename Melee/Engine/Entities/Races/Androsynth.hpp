#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

#include "Engine/Util/Periodic.hpp"

namespace Melee
{
    class AndrosynthShipEntity : public ShipEntity
    {
    public:
        explicit    AndrosynthShipEntity(const Point& position);
        virtual     ~AndrosynthShipEntity() = default;

    // ShipEntity i/f:
    protected:
        void        update(Engine& engine, uint32_t msElapsed) override;
        bool        onPrimaryWeaponFired(Engine& engine) override;
        bool        onSpecialWeaponFired(Engine& engine) override;

    private:
        void        setBlazerMode(bool enabled);

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();

    private:
        bool        m_blazerMode = false;
        Periodic    m_blazerEnergyRequired;
    };

    class AndrosynthBubbleWeaponEntity : public WeaponEntity
    {
    public:
        explicit    AndrosynthBubbleWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position, const Vector2d& velocity, const Vector2d& heading);
        virtual     ~AndrosynthBubbleWeaponEntity() = default;

    protected:
        void        update(Engine& engine, uint32_t msElapsed) override;

    private:
        static WeaponEntity::WeaponProperties	MakeWeaponProperties();

    private:
        Periodic    m_bubbleRotator;
    };
}
