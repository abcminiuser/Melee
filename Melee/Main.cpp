#include "Engine/Engine.hpp"
#include "Renderer/SFML/SFMLRenderer.hpp"

using namespace Melee;

namespace
{
    void AddTestEntities(Engine& engine, Renderer& renderer)
    {
        const float scaleFactor = (float)renderer.currentScaleFactor();

        {
            PlayerEntity::PlayerProperties playerProps = {};
            playerProps.mass_kg = 9.718e5;
            playerProps.engineForce_N = 8e3;
            playerProps.maxVelocity = 0;
            playerProps.rotation_degPerSec = 100;
            playerProps.maxVelocity = 1000;
            playerProps.maxHealth = 20;
            playerProps.maxEnergy = 10;
            playerProps.radius_km = 1000;

            auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ 200 * scaleFactor, 200 * scaleFactor });
            engine.addEntity(player1);

            auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ 100 * scaleFactor, 100 * scaleFactor });
            engine.addEntity(player2);
        }

        {
            PlanetEntity::PlanetProperties planetProps = {};
            planetProps.mass_kg = 3.9736e16f;
            planetProps.radius_km = 6371;

            auto planet = std::make_shared<PlanetEntity>(planetProps, Point{ 400 * scaleFactor, 300 * scaleFactor });
            engine.addEntity(planet);
        }

        {
            AsteroidEntity::AsteroidProperties asteroidProps = {};
            asteroidProps.radius_km = 1000;
            asteroidProps.mass_kg = 1e5;
            asteroidProps.rotation_degPerSec = 20;

            auto asteroid = std::make_shared<AsteroidEntity>(asteroidProps, Point{ 800 * scaleFactor, 200 * scaleFactor }, Vector2d{ 10, 10 });
            engine.addEntity(asteroid);
        }
    }
}

int main(int argc, char* argv[])
{
    Engine   engine;
    Renderer renderer(engine);

    AddTestEntities(engine, renderer);

    return renderer.runModal();
}
