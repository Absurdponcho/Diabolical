#include <iostream>
#include "Logging/Logging.h"
#include "WindowManager.h"
#include "ECS/GameEntity.h"
#include <cassert>
#include "GameManager.h"
#include "Rendering/GameRendererComponent.h"
#include "ECS/GameComponent.h"
#undef main

int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);

    WindowManager WManager = WindowManager(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    GameEntity* Entity = CreateEntity<GameEntity>();
    GameRendererComponent* Renderer = CreateComponent<GameRendererComponent>(Entity);

    GameManager GManager;
    GManager.MainGameLoop();

    return 0;
} 
