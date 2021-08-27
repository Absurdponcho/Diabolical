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
    GameAssetSoftPointer<TextureAsset> PlayerTexturePointer("GameAssetFiles/Player.png");
    GameAssetSoftPointer<TextureAsset> ArcaneBulletTexturePointer("GameAssetFiles/ArcaneBullet.png");
    GameAssetSoftPointer<TextureAsset> PlayerArmTexturePointer("GameAssetFiles/PlayerArm.png");

    for (int i = 0; i < 3; i++)
    {
        GameEntity* Square0 = CreateEntity<GameEntity>();
        Square0->GetTransform().SetPosition(glm::vec3(sin(i) * 10.f, i / 10.f, 0));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square0);
        CreateComponent<SpriteRendererComponent>(Square0)->SetTexture(CrateTexturePointer);
        Rigidbody->SetDynamic(true);
    }

    GameEntity* Square1 = CreateEntity<GameEntity>();
    {
        Square1->GetTransform().SetPosition(glm::vec3(0, -1, 1));
        Square1->GetTransform().SetScale(glm::vec3(40, 1, 1));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Square1);
        CreateComponent<SpriteRendererComponent>(Square1)->SetTexture(CrateTexturePointer);
    }

    PlayerCharacterEntity* PlayerCharacter = CreateEntity<PlayerCharacterEntity>();
    {
        PlayerCharacter->GetTransform().SetPosition(glm::vec3(0, 1, 0));
        PlayerCharacter->GetTransform().SetScale(glm::vec3(1, 2, 1));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(PlayerCharacter);
        Rigidbody->SetDynamic(true);
        Rigidbody->SetRotates(false);
        Rigidbody->SetHorizonalDamping(2);
        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerCharacter);
        Sprite->SetTexture(PlayerTexturePointer);
        Sprite->SpriteSheetSize = glm::ivec2(2, 1);
        Sprite->SpriteSheetProgressionSpeed = 4;
    }

    GameEntity* PlayerArm = CreateEntity<GameEntity>();
    {
        PlayerArm->GetTransform().SetPosition(PlayerCharacter->GetTransform().GetPosition() + glm::vec3(0, 0, 0.01f));
        PlayerArm->GetTransform().SetScale(glm::vec3(1, 1, 1));
        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerArm);
        Sprite->SetTexture(PlayerArmTexturePointer);

    }
    

    PlayerTrackerEntity* PlayerTracker = CreateEntity<PlayerTrackerEntity>();
    {
        PlayerTracker->SetTrackTarget(PlayerCharacter);
        CreateComponent<CameraComponent>(PlayerTracker)->SetActiveCamera();
    }

    
    GameEntity* ArcaneBullet = CreateEntity<GameEntity>();
    {
        ArcaneBullet->GetTransform().SetPosition(glm::vec3(0, 2, -0.1f));
        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(ArcaneBullet);
        Sprite->SetTexture(ArcaneBulletTexturePointer);
        Sprite->SpriteSheetSize = glm::ivec2(4, 1);
        Sprite->SpriteSheetProgressionSpeed = 8;
        Sprite->bMirrored = false;
    }

    GManager.MainGameLoop();


    return 0;
} 
