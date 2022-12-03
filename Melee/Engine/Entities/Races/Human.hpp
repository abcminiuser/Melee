#pragma once

#include "Engine/Entities/ShipEntity.hpp"
#include "Engine/Entities/WeaponEntity.hpp"

namespace Melee
{
    class HumanShipEntity : public ShipEntity
    {
    public:
        explicit    HumanShipEntity(const Point& position);
        virtual     ~HumanShipEntity() = default;

    // ShipEntity i/f:
    protected:
        bool        onPrimaryWeaponFired(Engine& engine) override;
        bool        onSpecialWeaponFired(Engine& engine) override;

    private:
        static ShipEntity::ShipProperties		MakeShipProperties();
    };

    class HumanMissileWeaponEntity : public WeaponEntity
    {
    public:
        explicit    HumanMissileWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position, const Vector2d& velocity, const Vector2d& heading);
        virtual     ~HumanMissileWeaponEntity() = default;

    private:
        static WeaponEntity::WeaponProperties	MakeWeaponProperties();
    };

    class HumanLaserWeaponEntity : public WeaponEntity
    {
    public:
        explicit    HumanLaserWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position);
        virtual     ~HumanLaserWeaponEntity() = default;

    private:
        static WeaponEntity::WeaponProperties	MakeWeaponProperties();
    };
}
