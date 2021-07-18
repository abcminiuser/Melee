#include "PlayerEntity.hpp"

#include <cmath>

using namespace Melee;

PlayerEntity::PlayerEntity(int playerIndex, const Point& pos)
	: Entity(Entity::Type::Player, pos)
	, m_playerIndex(playerIndex)
{
	m_heading = RotationMatrix(32) * Vector2d{ 1, 0 };
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
		static const auto rotateLeft = RotationMatrix(-1);
		m_heading = rotateLeft * m_heading;
	}
	else if (rotateFlags == Flags::RotateRightActive)
	{
		static const auto rotateRight = RotationMatrix(1);
		m_heading = rotateRight * m_heading;
	}

	const auto thrustFlags = m_flags & (Flags::ThrustActive | Flags::ReverseThrustActive);
	if (thrustFlags == Flags::ThrustActive)
		m_acceleration = m_heading * 10;
	else if (thrustFlags == Flags::ReverseThrustActive)
		m_acceleration = -m_heading * 10;
	else
		m_acceleration = Vector2d{ 0, 0 };

	Entity::update(msElapsed);
}
