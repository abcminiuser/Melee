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
            auto player1 = std::make_shared<Race1ShipEntity>(0, Point{ playFieldSize * .2f, playFieldSize * .2f });
            engine.addEntity(player1);

            auto player2 = std::make_shared<Race2ShipEntity>(1, Point{ playFieldSize * .1f, playFieldSize * .1f });
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
    AsteroidGenerator asteroidGenerator(engine, 1000);
    SFMLRenderer      renderer(engine);
    SFMLAudio         audio(engine);

    asteroidGenerator.setMaxAsteroids(2);
    asteroidGenerator.setVelocityRange(10, 20);
    asteroidGenerator.setRadiusRange(1000, 1100);

    audio.setMusicVolume(20);
    audio.setSoundEffectVolume(100);

    AddTestEntities(engine);

    return renderer.runModal();
}
