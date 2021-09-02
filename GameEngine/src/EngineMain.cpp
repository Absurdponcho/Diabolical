#include "CoreEngine.h"
#include "Rendering/RenderPipeline.h"
#include "Physics/ColliderComponent.h"
#include "Utility/GameMath.h"
#include "Rendering/TextRendererComponent.h"
#include "EngineMain.h"
#include "GameManager.h"
#undef main

GameManager* Manager = nullptr;

void InitializeEngine(int argc, char** argv)
{

    
    WindowManager::Initialize(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1600, 800, SDL_WINDOW_HIDDEN);

    PhysicsWorld::Initialize(b2Vec2(0, -13.8f));
    ParticleManager::Initialize();
    GlobalRandom::Initialize();

    Manager = new GameManager();
} 

void RunEngineLoop()
{
    Check(Manager);
    Manager->MainGameLoop();
}