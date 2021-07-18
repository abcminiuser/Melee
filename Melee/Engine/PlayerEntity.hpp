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

	public:
		explicit	PlayerEntity(int playerIndex, const Point& pos);
		virtual		~PlayerEntity() = default;

		int			index() const		{ return m_playerIndex; }
		void		handleKey(KeyEvent key, bool down);

	// Entity i/f:
	public:
		void		update(uint32_t msElapsed) override;

	private:
		const int	m_playerIndex;

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

		uint32_t	m_flags = 0;
	};
}
