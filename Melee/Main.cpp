#include "Engine/Engine.hpp"
#include "Engine/Generators/AsteroidGenerator.hpp"
#include "Engine/Generators/PlanetGenerator.hpp"

#include "Audio/SFML/SFMLAudio.hpp"

#include "Renderer/SFML/SFMLRenderer.hpp"

using namespace Melee;

namespace
{
    constexpr auto kPlayfieldSize = 200000;
}

int main(int argc, char* argv[])
{
    Engine            engine(kPlayfieldSize);
    PlanetGenerator   planetGenerator(engine);
    AsteroidGenerator asteroidGenerator(engine);
    SFMLRenderer      renderer(engine);
    SFMLAudio         audio(engine);

    auto player1 = std::make_shared<Race1ShipEntity>(Point{ kPlayfieldSize * .2f, kPlayfieldSize * .2f });
    engine.addEntity(player1);

    auto player2 = std::make_shared<Race2ShipEntity>(Point{ kPlayfieldSize * .1f, kPlayfieldSize * .1f });
    engine.addEntity(player2);

    engine.update(0);

    planetGenerator.setMassRange(3.9736e16f, 5.9736e16f);
    planetGenerator.setRadiusRange(4000, 6000);
    planetGenerator.generate();

    asteroidGenerator.setVelocityRange(10, 20);
    asteroidGenerator.setRadiusRange(1000, 1100);
    asteroidGenerator.setPeriodicGeneration(2, 1000);

    audio.setMasterVolume(100);
    audio.setMusicVolume(20);

    return renderer.runModal();
}
