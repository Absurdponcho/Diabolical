#include "CoreEngine.h"

#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Default);

    WindowManager::Initialize(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 800, SDL_WINDOW_HIDDEN);

    PhysicsWorld::Initialize(b2Vec2(0, -13.8f));
   
    ParticleManager::Initialize();

    InputManager::AddKeyMapping("Jump", SDLK_SPACE);
    InputManager::AddKeyMapping("Jump", SDLK_UP);
    InputManager::AddKeyMapping("Jump", SDLK_w);
    InputManager::AddKeyMapping("Right", SDLK_d);
    InputManager::AddKeyMapping("Right", SDLK_RIGHT);
    InputManager::AddKeyMapping("Left", SDLK_LEFT);
    InputManager::AddKeyMapping("Left", SDLK_a);

    GameEntity* Test = CreateEntity<GameEntity>();

    GameManager GManager;

    GameAssetSoftPointer<TextureAsset> CrateTexturePointer("GameAssetFiles/Crate.png");

    for (int i = 5; i < 10; i++)
    {
        GameEntity* Square0 = CreateEntity<GameEntity>();
        Square0->GetTransform().Position = glm::vec3(sin(i) * 10.f, i / 5.f, 0);
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square0);
        CreateComponent<SpriteRendererComponent>(Square0)->SetTexture(CrateTexturePointer);
        Rigidbody->SetDynamic(true);
        InputManager::BindMethod("TestAction", Rigidbody, &RigidbodyComponent::Jump);
    }
    InputManager::AddMouseButtonMapping("TestAction", SDL_BUTTON_LEFT);

    GameEntity* Square1 = CreateEntity<GameEntity>();
    {
        Square1->GetTransform().Position = glm::vec3(0, -6, 0);
        Square1->GetTransform().Scale = glm::vec3(40, 1, 1);
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square1);
        CreateComponent<SpriteRendererComponent>(Square1)->SetTexture(CrateTexturePointer);
    }

    PlayerCharacterEntity* PlayerCharacter = CreateEntity<PlayerCharacterEntity>();
    {
        PlayerCharacter->GetTransform().Position = glm::vec3(0, 10, 0);
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(PlayerCharacter);
        Rigidbody->SetDynamic(true);
        Rigidbody->SetRotates(false);
        Rigidbody->SetHorizonalDamping(200);
        CreateComponent<SpriteRendererComponent>(PlayerCharacter)->SetTexture(CrateTexturePointer);
    }

    PlayerTrackerEntity* PlayerTracker = CreateEntity<PlayerTrackerEntity>();
    {
        PlayerTracker->SetTrackTarget(PlayerCharacter);
        CreateComponent<CameraComponent>(PlayerTracker)->SetActiveCamera();
    }
    

    GManager.MainGameLoop();


    return 0;
} 
