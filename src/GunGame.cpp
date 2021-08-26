#include "CoreEngine.h"
#include "Rendering/BasicRendererComponent.h"
#include "Rendering/SquareRendererComponent.h"
#include "Physics/RigidbodyComponent.h"
#include "Textures/TextureAsset.h"
#include "Rendering/SpriteRendererComponent.h"
#include "Rendering/Particles/Particle.h"
#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Default);

    WindowManager::Initialize(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 800, SDL_WINDOW_HIDDEN);

    PhysicsWorld::Initialize(b2Vec2(0, -9.8f));
   
    ParticleManager::Initialize();

    InputManager::AddKeyMapping("Jump", SDLK_SPACE);

    GameEntity* Test = CreateEntity<GameEntity>();

    GameManager GManager;

    GameAssetSoftPointer<TextureAsset> CrateTexturePointer("GameAssetFiles/Crate.png");

    for (int i = 10; i < 100; i++)
    {
        GameEntity* Square0 = CreateEntity<GameEntity>();
        Square0->GetTransform().Position = glm::vec3(sin(i) * 10.f, i / 5.f, 0);
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square0);
        CreateComponent<SpriteRendererComponent>(Square0)->SetTexture(CrateTexturePointer);
        Rigidbody->SetDynamic(true);
    }

    GameEntity* Square1 = CreateEntity<GameEntity>();
    Square1->GetTransform().Position = glm::vec3(0, -6, 0);
    Square1->GetTransform().Scale = glm::vec3(40, 1, 1);
    RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square1);
    CreateComponent<SpriteRendererComponent>(Square1)->SetTexture(CrateTexturePointer);

    GameEntity* Player = CreateEntity<GameEntity>();
    CreateComponent<CameraComponent>(Player)->SetActiveCamera();



    GManager.MainGameLoop();


    return 0;
} 
