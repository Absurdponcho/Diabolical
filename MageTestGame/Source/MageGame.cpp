#include <EngineMain.h>
#include <Logging/Logging.h>
#include <Rendering/RenderPipeline.h>
#include <Input/InputManager.h>
#include <Assets/GameAssetSoftPointer.h>
#include "PlayerCharacter/PlayerCharacterEntity.h"
#include <Physics/RigidbodyComponent.h>
#include <Physics/ColliderComponent.h>
#include <Rendering/SpriteRendererComponent.h>
#include <Rendering/Camera.h>
#include "PlayerCharacter/PlayerTrackerEntity.h"
#include <Audio/GameAudio.h>
#include "Enemy/EnemyEntity.h"

#undef main

int main(int argc, char** argv)
{
	Logging::SetLogVerbosity(ELogVerbosity::LV_Default);

	InitializeEngine(argc, argv);

	RenderPipeline DefaultRenderPipeline = RenderPipeline();
	DefaultRenderPipeline.Bind();

    InputManager::AddKeyMapping("Up", SDLK_UP);
    InputManager::AddKeyMapping("Up", SDLK_w);
    InputManager::AddKeyMapping("Right", SDLK_d);
    InputManager::AddKeyMapping("Right", SDLK_RIGHT);
    InputManager::AddKeyMapping("Left", SDLK_LEFT);
    InputManager::AddKeyMapping("Left", SDLK_a);    
    InputManager::AddKeyMapping("Down", SDLK_DOWN);
    InputManager::AddKeyMapping("Down", SDLK_s);
    InputManager::AddMouseButtonMapping("AttackPrimary", SDL_BUTTON_LEFT);
    InputManager::AddMouseButtonMapping("AttackSecondary", SDL_BUTTON_RIGHT);

    GameAssetSoftPointer<TextureAsset> PlayerTexturePointer("GameAssetFiles/FixedMage.png");
    GameAssetSoftPointer<TextureAsset> GroundTileTexturePointer("GameAssetFiles/GroundTile.png");
    glBindTexture(GL_TEXTURE_2D, GroundTileTexturePointer.LoadSynchronous()->GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    auto Music = GameAssetSoftPointer<AudioAsset>("GameAssetFiles/Music/Nightmares Inn - RKVC.wav");
    GameAudio::PlaySound(Music, .2f, 10, true);

    PlayerCharacterEntity* PlayerCharacter = CreateEntity<PlayerCharacterEntity>();
    {
       // PlayerCharacter->GetTransform().SetPosition(glm::vec3(0, 0, 0));
        PlayerCharacter->GetTransform().SetScale(glm::vec3(2, 2, 2));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(PlayerCharacter);
        Rigidbody->SetDynamic(true);
        Rigidbody->SetRotates(false);
        Rigidbody->SetHorizonalDamping(0);
        Rigidbody->SetOffset(glm::vec2(0, .5f));
        Rigidbody->SetUsesGravity(false);
        ColliderComponent* Collider = CreateComponent<ColliderComponent>(PlayerCharacter);
        Collider->SetSize(b2Vec2(.2f, .2f));
        Collider->SetDensity(3);

        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerCharacter);
        Sprite->SetTexture(PlayerTexturePointer);
        Sprite->SpriteSheetSize = glm::ivec2(8, 8);
        Sprite->SpriteSheetProgressionSpeed = 8;
        Sprite->bUseSpriteLoop = true;
        Sprite->SpriteLoopStart = 0;
        Sprite->SpriteLoopEnd = 4;
        Sprite->bUseYAsDepth = true;

    }

    PlayerTrackerEntity* Camera = CreateEntity<PlayerTrackerEntity>();
    {
        CreateComponent<CameraComponent>(Camera)->SetActiveCamera();

        Camera->SetTrackTarget(PlayerCharacter);
    }

    GameEntity* Background = CreateEntity<GameEntity>();
    {
        Background->GetTransform().SetPosition(glm::vec3(0, 0, -.99999));
        Background->GetTransform().SetScale(glm::vec3(100, 100, 1));

        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(Background);
        Sprite->SetTexture(GroundTileTexturePointer);
        Sprite->SetTextureScaling(glm::vec2(100, 100));
    }

    GameAssetSoftPointer<TextureAsset> SkeletonIdleTexturePointer("GameAssetFiles/Skele/Sprite Sheets/Skeleton Idle.png");

    for (int x = -20; x < 20; x++)
    {
        for (int y = -20; y < 20; y++)
        {
            EnemyEntity* Skele = CreateEntity<EnemyEntity>();
            {
                Skele->GetTransform().SetScale(glm::vec3(1, 1, 1));
                Skele->GetTransform().SetPosition(glm::vec3(x / 2, y / 2, 0));

                RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(Skele);
                Rigidbody->SetDynamic(true);
                Rigidbody->SetRotates(false);
                Rigidbody->SetHorizonalDamping(0);
                Rigidbody->SetOffset(glm::vec2(0, .5f));
                Rigidbody->SetUsesGravity(false);
                Rigidbody->SetLinearDamping(12.f);

                ColliderComponent* Collider = CreateComponent<ColliderComponent>(Skele);
                Collider->SetSize(b2Vec2(.2f, .2f));
                Collider->SetDensity(3);

                SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(Skele);
                Sprite->SetTexture(SkeletonIdleTexturePointer);
                Sprite->bUseSpriteLoop = true;
                Sprite->SpriteLoopStart = 0;
                Sprite->SpriteLoopEnd = 11;
                Sprite->SpriteSheetSize = glm::ivec2(11, 1);
                Sprite->SpriteSheetProgressionSpeed = 12;
                Sprite->bUseYAsDepth = true;

            }
        }
    }

    RunEngineLoop();

    return 0;
}