#include <iostream>
#include "Logging/Logging.h"
#include "WindowManager.h"
#include "ECS/GameEntity.h"
#include <cassert>
#include "GameManager.h"
#include "ECS/GameComponent.h"
#include "Rendering/TextRendererComponent.h"
#include <filesystem>
#include "Assets/GameAssetSoftPointer.h"
#undef main

int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);

    WindowManager WManager = WindowManager(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    GameEntity* Entity = CreateEntity<GameEntity>();
    TextRendererComponent* Renderer = CreateComponent<TextRendererComponent>(Entity);

    GameAssetSoftPointer TestAssetPtr = GameAssetSoftPointer("GameAssetFiles/Test.txt");
    GameAsset* TestAsset = TestAssetPtr.LoadSynchronous();
    Check(TestAsset);

    GameManager GManager;
    GManager.MainGameLoop();

    return 0;
} 
