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
    glBindTexture(GL_TEXTURE_2D, PlayerTexturePointer.LoadSynchronous()->GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    PlayerCharacterEntity* PlayerCharacter = CreateEntity<PlayerCharacterEntity>();
    {
        PlayerCharacter->GetTransform().SetPosition(glm::vec3(0, 0, 0));
        PlayerCharacter->GetTransform().SetScale(glm::vec3(2, 2, 2));
        RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(PlayerCharacter);
        Rigidbody->SetDynamic(true);
        Rigidbody->SetRotates(false);
        Rigidbody->SetHorizonalDamping(0);
        Rigidbody->SetOffset(glm::vec2(0, .5f));
        Rigidbody->SetUsesGravity(false);
        ColliderComponent* Collider = CreateComponent<ColliderComponent>(PlayerCharacter);
        Collider->SetSize(b2Vec2(.2f, .5f));
        Collider->SetDensity(3);

        SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(PlayerCharacter);
        Sprite->SetTexture(PlayerTexturePointer);
        Sprite->SpriteSheetSize = glm::ivec2(8, 8);
        Sprite->SpriteSheetProgressionSpeed = 8;
        Sprite->bUseSpriteLoop = true;
        Sprite->SpriteLoopStart = 0;
        Sprite->SpriteLoopEnd = 4;

    }

    GameEntity* Camera = CreateEntity<GameEntity>();
    {
        CreateComponent<CameraComponent>(Camera)->SetActiveCamera();
    }

    RunEngineLoop();

    return 0;
}