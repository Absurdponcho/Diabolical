#include "CoreEngine.h"
#include "Rendering/BasicRendererComponent.h"
#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);

    WindowManager::Initialize(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    GameManager GManager;

    GameEntity* Entity = CreateEntity<GameEntity>();
    BasicRendererComponent* Component = CreateComponent<BasicRendererComponent>(Entity);
    CreateComponent<SquareRendererComponent>(Entity);
    Entity->GetTransform().Scale = b2Vec2(.1f, .1f);

    GManager.MainGameLoop();


    return 0;
} 
