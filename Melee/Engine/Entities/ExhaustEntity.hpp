#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
	class ExhaustEntity : public Entity
	{
	public:
		struct ExhaustProperties : public Properties
		{
			uint32_t maxAge = 1000;
		};

	public:
		explicit	ExhaustEntity(const ExhaustProperties& properties, const Point& pos, const Vector2d& heading);
		virtual		~ExhaustEntity() = default;

		const auto& properties() const	{ return m_exhaustProperties; }

		uint32_t	age() const			{ return m_age; }

	// Entity i/f:
	public:
		void		update(Engine& engine, uint32_t msElapsed) override;

	private:
		const ExhaustProperties	m_exhaustProperties;

		uint32_t				m_age = 0;
	};
}
