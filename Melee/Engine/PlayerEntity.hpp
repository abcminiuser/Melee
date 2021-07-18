#pragma once

#include "Entity.hpp"

namespace Melee
{
	class PlayerEntity : public Entity
	{
	public:
		enum class KeyEvent
		{
			Thrust,
			RotateLeft,
			RotateRight,
			ReverseThrust,
			FirePrimary,
			FireSpecial,
		};

		struct PlayerProperties : Properties
		{
			float		engineForce_N		= 0;
			float		rotation_degPerSec	= 0;
		};

	public:
		explicit	PlayerEntity(int playerIndex, const PlayerProperties& properties, const Point& pos);
		virtual		~PlayerEntity() = default;

		int			index() const		{ return m_playerIndex; }
		void		handleKey(KeyEvent key, bool down);

	// Entity i/f:
	public:
		void		update(uint32_t msElapsed) override;

	private:
		struct Flags
		{
			enum Mask : uint32_t
			{
				ThrustActive			= 1 << 0,
				ReverseThrustActive		= 1 << 1,
				RotateLeftActive		= 1 << 2,
				RotateRightActive		= 1 << 3,
			};
		};

		const int				m_playerIndex;
		const PlayerProperties	m_playerProperties;

		float					m_engineAcceleration_ms2;
		Matrix2x2				m_rotationalThrustLeft;
		Matrix2x2				m_rotationalThrustRight;

		uint32_t				m_flags = 0;
		uint32_t				m_rotationMsElapsed = 0;
	};
}
