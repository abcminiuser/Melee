#pragma once

#include "Engine/Engine.hpp"

namespace Melee
{
	class AsteroidGenerator : public Engine::Observer
	{
	public:
	    explicit    	AsteroidGenerator(Engine& engine, uint8_t maxAsteroids, float minVelocity_km_s, float maxVelocity_km_s, float minRadius_km, float maxRadius_km, uint32_t generationIntervalMs = 5000);
	    virtual     	~AsteroidGenerator();

	// Engine::Observer i.f:
	public:
	    void        	updated(Engine& engine, uint32_t msElapsed) override;
		void			entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity) override;

	private:
	    Engine&         m_engine;

	    const uint8_t   m_maxAsteroids = 0;

	    const float     m_minVelocity_km_s = 0;
	    const float     m_maxVelocity_km_s = 0;

	    const float     m_minRadius_km = 0;
	    const float     m_maxRadius_km = 0;

	    Periodic        m_generatorTimer;
	    size_t          m_currentTotalAsteroids = 0;
	};
}
