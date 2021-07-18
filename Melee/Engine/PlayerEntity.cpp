#include "PlayerEntity.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
	constexpr uint32_t kRotationIntervalMs = 10;
}

PlayerEntity::PlayerEntity(int playerIndex, const PlayerProperties& properties, const Point& pos)
	: Entity(Entity::Type::Player, properties, pos)
	, m_playerIndex(playerIndex)
	, m_playerProperties(properties)
{
	m_engineAcceleration_ms2	= properties.engineForce_N / properties.mass_kg;

	const float rotationDegreesPerInterval = properties.rotation_degPerSec * kRotationIntervalMs / 1000;
	m_rotationalThrustLeft		= RotationMatrix(-rotationDegreesPerInterval);
	m_rotationalThrustRight		= RotationMatrix(rotationDegreesPerInterval);
}

void PlayerEntity::handleKey(KeyEvent key, bool down)
{
	const auto UpdateFlag = [&](Flags::Mask flag, bool state)
		{
			if (state)
				m_flags |= flag;
			else
				m_flags &= ~flag;
		};

	if (key == KeyEvent::Thrust)
		UpdateFlag(Flags::ThrustActive, down);
	else if (key == KeyEvent::ReverseThrust)
		UpdateFlag(Flags::ReverseThrustActive, down);
	else if (key == KeyEvent::RotateLeft)
		UpdateFlag(Flags::RotateLeftActive, down);
	else if (key == KeyEvent::RotateRight)
		UpdateFlag(Flags::RotateRightActive, down);
}

void PlayerEntity::update(uint32_t msElapsed)
{
	const auto rotateFlags = m_flags & (Flags::RotateLeftActive | Flags::RotateRightActive);
	if (rotateFlags == Flags::RotateLeftActive)
	{
		m_rotationMsElapsed += msElapsed;

		while (m_rotationMsElapsed > kRotationIntervalMs)
		{
			m_heading = m_rotationalThrustLeft * m_heading;
			m_rotationMsElapsed -= std::min(kRotationIntervalMs, m_rotationMsElapsed);
		}
	}
	else if (rotateFlags == Flags::RotateRightActive)
	{
		m_rotationMsElapsed += msElapsed;

		while (m_rotationMsElapsed > kRotationIntervalMs)
		{
			m_heading = m_rotationalThrustRight * m_heading;
			m_rotationMsElapsed -= std::min(kRotationIntervalMs, m_rotationMsElapsed);
		}
	}
	else
	{
		m_rotationMsElapsed = 0;
	}

	const auto thrustFlags = m_flags & (Flags::ThrustActive | Flags::ReverseThrustActive);
	if (thrustFlags == Flags::ThrustActive)
		m_acceleration = m_heading * m_engineAcceleration_ms2;
	else if (thrustFlags == Flags::ReverseThrustActive)
		m_acceleration = -m_heading * m_engineAcceleration_ms2;
	else
		m_acceleration = Vector2d{ 0, 0 };

	Entity::update(msElapsed);
}
