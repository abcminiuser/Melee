#include "Engine/Engine.hpp"
#include "Audio/SFML/SFMLAudio.hpp"
#include "Renderer/SFML/SFMLRenderer.hpp"
#include "AsteroidGenerator.hpp"

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
            playerProps.maxVelocity_km_s = 40;
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
}

int main(int argc, char* argv[])
{
    Engine            engine(200000);
    AsteroidGenerator asteroidGenerator(engine, 2, 10, 20, 1000, 1200);
    SFMLRenderer      renderer(engine);
    SFMLAudio         audio(engine);

    audio.setVolume(30);

    AddTestEntities(engine);

    return renderer.runModal();
}
