
#include <EngineMain.h>
#include <Logging/Logging.h>
#include <Rendering/RenderPipeline.h>
#include <Input/InputManager.h>
#include <ECS/GameEntity.h>
#include "PlayerCharacter/PlayerCharacterEntity.h"
#include <Physics/RigidbodyComponent.h>
#include <Physics/ColliderComponent.h>
#include "PlayerCharacter/PlayerTrackerEntity.h"
#include <Rendering/SpriteRendererComponent.h>
#include <Rendering/TextRendererComponent.h>
#include <Rendering/Camera.h>



#undef main

int main(int argc, char** argv)
{
	Logging::SetLogVerbosity(ELogVerbosity::LV_Default);

	InitializeEngine(argc, argv);

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

    GameEntity* TextTest = CreateEntity<GameEntity>();
    {
        TextTest->GetTransform().SetPosition(glm::vec3(0, 5, -1.f));
        TextRendererComponent* TextRenderer = CreateComponent<TextRendererComponent>(TextTest);
        TextRenderer->SetFont(GameAssetSoftPointer<FreetypeFontAsset>("GameAssetFiles/Fonts/GothicPixel/GothicPixelRegular-j1dG.ttf"));
        TextRenderer->Text = "Welcome to the game!";
        TextRenderer->ForegroundColor = glm::vec3(.3f, .5, .3f);
        TextRenderer->ShadowColor = glm::vec3(.3f, .5, .3f) / 2.f;
        TextRenderer->TextSize = 16;
        TextRenderer->ShadowOffset = glm::vec2(-2, -2);
        TextRenderer->bShadow = true;
    }

    RunEngineLoop();

	return 0;
}