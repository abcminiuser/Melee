#include "Engine/Engine.hpp"
#include "Renderer/SDL/SDLRenderer.hpp"

using namespace Melee;

int main(int argc, char* argv[])
{
    Engine      engine;
    SDLRenderer renderer(engine);

    return renderer.runModal();
}
