#include "CoreEngine.h"
#include "Rendering/RenderPipeline.h"
#include "Physics/ColliderComponent.h"
#include "Utility/GameMath.h"
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
    GlobalRandom::Initialize();

    RenderPipeline DefaultRenderPipeline = RenderPipeline();
    DefaultRenderPipeline.Bind();

    InputManager::AddKeyMapping("Jump", SDLK_SPACE);
    InputManager::AddKeyMapping("Jump", SDLK_UP);
    InputManager::AddKeyMapping("Jump", SDLK_w);
    InputManager::AddKeyMapping("Right", SDLK_d);
    InputManager::AddKeyMapping("Right", SDLK_RIGHT);
    InputManager::AddKeyMapping("Left", SDLK_LEFT);
    InputManager::AddKeyMapping("Left", SDLK_a);
    InputManager::AddMouseButtonMapping("Shoot", SDL_BUTTON_LEFT);

    GameEntity* Test = CreateEntity<GameEntity>();

    GameManager GManager;

    GameAssetSoftPointer<TextureAsset> CrateTexturePointer("GameAssetFiles/Crate.png");
    GameAssetSoftPointer<TextureAsset> PlayerTexturePointer("GameAssetFiles/Player.png");
    GameAssetSoftPointer<TextureAsset> PlayerArmTexturePointer("GameAssetFiles/PlayerArm.png");
    GameAssetSoftPointer<TextureAsset> GunTexturePointer("GameAssetFiles/Gun.png");

    for (int i = 0; i < 3; i++)
    {
        GameEntity* Square0 = CreateEntity<GameEntity>();
        Square0->GetTransform().SetPosition(glm::vec3(sin(i) * 10.f, i / 10.f, 0));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square0);
        CreateComponent<ColliderComponent>(Square0);
        CreateComponent<SpriteRendererComponent>(Square0)->SetTexture(CrateTexturePointer);
        Rigidbody->SetDynamic(true);
    }

    GameEntity* Floor = CreateEntity<GameEntity>();
    {
        Floor->GetTransform().SetPosition(glm::vec3(0, -1, 1));
        Floor->GetTransform().SetScale(glm::vec3(40, 1, 1));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Floor);
        CreateComponent<ColliderComponent>(Floor)->SetSize(b2Vec2(20.f, .5f));
        CreateComponent<SpriteRendererComponent>(Floor)->SetTexture(CrateTexturePointer);
    }

    PlayerCharacterEntity* PlayerCharacter = CreateEntity<PlayerCharacterEntity>();
    {
        PlayerCharacter->GetTransform().SetPosition(glm::vec3(0, 0, 0));
        PlayerCharacter->GetTransform().SetScale(glm::vec3(2, 2, 2));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(PlayerCharacter);
        Rigidbody->SetDynamic(true);
        Rigidbody->SetRotates(false);
        Rigidbody->SetHorizonalDamping(2);
        Rigidbody->SetOffset(glm::vec2(0, .5f));
        ColliderComponent* Collider = CreateComponent<ColliderComponent>(PlayerCharacter);
        Collider->SetSize(b2Vec2(.2f, .5f));
        Collider->SetDensity(3);

        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerCharacter);
        Sprite->SetTexture(PlayerTexturePointer);
        Sprite->SpriteSheetSize = glm::ivec2(2, 1);
        Sprite->SpriteSheetProgressionSpeed = 4;
    }

    PlayerCharacter->PlayerArm = CreateEntity<GameEntity>();
    {
        PlayerCharacter->PlayerArm->GetTransform().SetPosition(glm::vec3(0, -0.05f, 0.01f));
        PlayerCharacter->PlayerArm->GetTransform().SetScale(glm::vec3(.5, .5, .5));
        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerCharacter->PlayerArm);
        Sprite->SetTexture(PlayerArmTexturePointer);
        PlayerCharacter->PlayerArm->GetTransform().AttachTo(PlayerCharacter);
    }
    
    PlayerCharacter->HeldItem = CreateEntity<GameEntity>();
    {
        PlayerCharacter->HeldItem->GetTransform().SetPosition(glm::vec3(0, -0.55f, 0.02f));
        PlayerCharacter->HeldItem->GetTransform().SetEulerRotation(glm::vec3(0, 0, -90));
        PlayerCharacter->HeldItem->GetTransform().SetScale(glm::vec3(.7f, .7f, .7f));
        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerCharacter->HeldItem);
        Sprite->SetTexture(GunTexturePointer);
        PlayerCharacter->HeldItem->GetTransform().AttachTo(PlayerCharacter->PlayerArm);
    }

    PlayerTrackerEntity* PlayerTracker = CreateEntity<PlayerTrackerEntity>();
    {
        PlayerTracker->SetTrackTarget(PlayerCharacter);
        CreateComponent<CameraComponent>(PlayerTracker)->SetActiveCamera();
    }


    GManager.MainGameLoop();


    return 0;
} 
