#include "CoreEngine.h"
#include "Rendering/BasicRendererComponent.h"
#include "Rendering/SquareRendererComponent.h"
#include "Rendering/Camera.h"
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
    CreateComponent<BasicRendererComponent>(Entity);
    
    GameEntity* Square0 = CreateEntity<GameEntity>();
    Square0->GetTransform().Position = glm::vec3(0, 0, 0);
    CreateComponent<SquareRendererComponent>(Entity);  

    GameEntity* Square1 = CreateEntity<GameEntity>();
    Square1->GetTransform().Position = glm::vec3(-1, 0, 0);
    CreateComponent<SquareRendererComponent>(Square1);

    GameEntity* Square2 = CreateEntity<GameEntity>();
    Square2->GetTransform().Position = glm::vec3(-1, -1, 0);
    CreateComponent<SquareRendererComponent>(Square2);

    GameEntity* Square3 = CreateEntity<GameEntity>();
    Square3->GetTransform().Position = glm::vec3(1, -1, 0);
    CreateComponent<SquareRendererComponent>(Square3);

    GameEntity* Player = CreateEntity<GameEntity>();
    CreateComponent<CameraComponent>(Player)->SetActiveCamera();

    GManager.MainGameLoop();


    return 0;
} 
