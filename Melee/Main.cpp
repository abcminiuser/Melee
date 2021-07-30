#include "Engine/Engine.hpp"
#include "Renderer/SFML/SFMLRenderer.hpp"

using namespace Melee;

namespace
{
    void AddTestEntities(Engine& engine)
    {
        const auto playFieldSize = engine.getPlayfieldSize();

        {
            PlayerEntity::PlayerProperties playerProps = {};
            playerProps.mass_kg = 9.718e5;
            playerProps.engineForce_N = 8e3;
            playerProps.rotation_degPerSec = 100;
            playerProps.maxVelocity_km_s = 100;
            playerProps.maxHealth = 2;
            playerProps.maxEnergy = 10;
            playerProps.radius_km = 1000;

            auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ playFieldSize * .2f, playFieldSize * .2f });
            engine.addEntity(player1);

            auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ playFieldSize * .1f, playFieldSize * .1f });
            engine.addEntity(player2);
        }

        {
            PlanetEntity::PlanetProperties planetProps = {};
            planetProps.mass_kg = 3.9736e16f;
            planetProps.radius_km = 6371;

            auto planet = std::make_shared<PlanetEntity>(planetProps, Point{ playFieldSize * .6f, playFieldSize * .3f });
            engine.addEntity(planet);
        }
    }

    class AsteroidGenerator : public Engine::Observer
    {
    public:
        explicit    AsteroidGenerator(uint8_t maxAsteroids, float minVelocity_km_s, float maxVelocity_km_s, float minRadius_km, float maxRadius_km)
            : m_maxAsteroids(maxAsteroids)
            , m_minVelocity_km_s(minVelocity_km_s)
            , m_maxVelocity_km_s(maxVelocity_km_s)
            , m_minRadius_km(minRadius_km)
            , m_maxRadius_km(maxRadius_km)
        {

        }

        virtual     ~AsteroidGenerator() = default;

        void        entitiesAdjusted(Engine& engine) override
        {
            const auto asteroidCount = engine.getEntities(Entity::Type::Asteroid).size();

            for (auto i = asteroidCount; i < m_maxAsteroids; i++)
            {
                AsteroidEntity::AsteroidProperties asteroidProps = {};
                asteroidProps.radius_km = LinearInterpolateRandom(m_minRadius_km, m_maxRadius_km);
                asteroidProps.rotation_degPerSec = LinearInterpolateRandom(-20.0f, 20.0f);
                asteroidProps.mass_kg = 1e5;

                const auto newSpeed = LinearInterpolateRandom(m_minVelocity_km_s, m_maxVelocity_km_s);
                const auto startingYPos = LinearInterpolateRandom(0.0f, engine.getPlayfieldSize());

                const auto velocity = RotationMatrix(LinearInterpolateRandom(0.0f, 360.0f)) * Vector2d{ newSpeed, 0 };
                const auto position = Point{ 0, startingYPos };

                auto asteroid = std::make_shared<AsteroidEntity>(asteroidProps, position, velocity);
                engine.addEntity(asteroid);
            }
        }

    private:
        const uint8_t                           m_maxAsteroids = 0;

        const float                             m_minVelocity_km_s = 0;
        const float                             m_maxVelocity_km_s = 0;

        const float                             m_minRadius_km = 0;
        const float                             m_maxRadius_km = 0;
    };
}

int main(int argc, char* argv[])
{
    Engine   engine(200000);
    Renderer renderer(engine);

    AsteroidGenerator asteroidGenerator(2, 10, 20, 1000, 1200);
    engine.addObserver(&asteroidGenerator);

    AddTestEntities(engine);

    return renderer.runModal();
}
