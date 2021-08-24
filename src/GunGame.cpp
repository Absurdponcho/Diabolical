#include "CoreEngine.h"
#include "Rendering/BasicRendererComponent.h"
#include "Rendering/SquareRendererComponent.h"
#include "Physics/RigidbodyComponent.h"
#include "Textures/TextureAsset.h"

#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Default);

    WindowManager::Initialize(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 800, SDL_WINDOW_HIDDEN, true);

    PhysicsWorld::Initialize(b2Vec2(0, -9.8f));
   
    InputManager::AddKeyMapping("Jump", SDLK_w);
    InputManager::AddKeyMapping("Jump", SDLK_UP);
    InputManager::AddKeyMapping("Jump", SDLK_SPACE);
    InputManager::AddKeyMapping("Left", SDLK_a);
    InputManager::AddKeyMapping("Left", SDLK_LEFT);
    InputManager::AddKeyMapping("Right", SDLK_d);
    InputManager::AddKeyMapping("Right", SDLK_RIGHT);    
    InputManager::AddKeyMapping("Down", SDLK_s);
    InputManager::AddKeyMapping("Down", SDLK_DOWN);

    GameEntity* Test = CreateEntity<GameEntity>();

    GameManager GManager;
    for (int i = 10; i < 20; i++)
    {
        GameEntity* Square0 = CreateEntity<GameEntity>();
        Square0->GetTransform().Position = glm::vec3(sin(i) * 10.f, i / 5.f, 0);
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square0);
        Rigidbody->SetDynamic(true);
        Rigidbody->bDrawDebugPolys = true;
    }

    GameEntity* Square1 = CreateEntity<GameEntity>();
    Square1->GetTransform().Position = glm::vec3(0, -6, 0);
    Square1->GetTransform().Scale = glm::vec3(40, 1, 1);
    RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square1);
    Rigidbody->bDrawDebugPolys = true;

    GameEntity* Player = CreateEntity<GameEntity>();
    CreateComponent<CameraComponent>(Player)->SetActiveCamera();
    GManager.MainGameLoop();


    return 0;
} 
