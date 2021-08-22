#include "CoreEngine.h"
#include "Rendering/BasicRendererComponent.h"
#include "Rendering/SquareRendererComponent.h"
#include "Physics/RigidbodyComponent.h"

#undef main

int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Default);

    WindowManager::Initialize(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 800, 0);

    PhysicsWorld::Initialize(b2Vec2(0, -9.8f));

    GameManager GManager;

    GameEntity* Entity = CreateEntity<GameEntity>();
    CreateComponent<BasicRendererComponent>(Entity);
    
    for (int i = 0; i < 100; i++)
    {
        GameEntity* Square0 = CreateEntity<GameEntity>();
        Square0->GetTransform().Position = glm::vec3(sin(i) * 10, i / 5, 0);
        CreateComponent<SquareRendererComponent>(Square0);
        CreateComponent<RigidbodyComponent>(Square0)->SetDynamic(true);
    }

    GameEntity* Square1 = CreateEntity<GameEntity>();
    Square1->GetTransform().Position = glm::vec3(0, -6, 0);
    Square1->GetTransform().Scale = glm::vec3(1, 1, 1);
    CreateComponent<SquareRendererComponent>(Square1);
    CreateComponent<RigidbodyComponent>(Square1);

 
    GameEntity* Player = CreateEntity<GameEntity>();
    CreateComponent<CameraComponent>(Player)->SetActiveCamera();

    GManager.MainGameLoop();


    return 0;
} 
