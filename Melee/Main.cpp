#include "Engine/Engine.hpp"
#include "Renderer/SFML/SFMLRenderer.hpp"

using namespace Melee;

namespace
{
    void AddTestEntities(Engine& engine, Renderer& renderer)
    {
        const auto playFieldSize = renderer.getPlayfieldSize();

        {
            PlayerEntity::PlayerProperties playerProps = {};
            playerProps.mass_kg = 9.718e5;
            playerProps.engineForce_N = 8e3;
            playerProps.maxVelocity = 0;
            playerProps.rotation_degPerSec = 100;
            playerProps.maxVelocity = 1000;
            playerProps.maxHealth = 2;
            playerProps.maxEnergy = 10;
            playerProps.radius_km = 1000;

            auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ playFieldSize.x * .2f, playFieldSize.y * .2f });
            engine.addEntity(player1);

            auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ playFieldSize.x * .1f, playFieldSize.y * .1f });
            engine.addEntity(player2);
        }

        {
            PlanetEntity::PlanetProperties planetProps = {};
            planetProps.mass_kg = 3.9736e16f;
            planetProps.radius_km = 6371;

            auto planet = std::make_shared<PlanetEntity>(planetProps, Point{ playFieldSize.x * .6f, playFieldSize.y * .3f });
            engine.addEntity(planet);
        }

        {
            AsteroidEntity::AsteroidProperties asteroidProps = {};
            asteroidProps.radius_km = 1000;
            asteroidProps.mass_kg = 1e5;
            asteroidProps.rotation_degPerSec = 20;

            auto asteroid = std::make_shared<AsteroidEntity>(asteroidProps, Point{ playFieldSize.x * .8f, playFieldSize.y * .2f }, Vector2d{ 10, 10 });
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
