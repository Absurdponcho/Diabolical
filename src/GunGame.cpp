#include "CoreEngine.h"
#include "Rendering/BasicRendererComponent.h"
#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);

    GameAssetSoftPointer<AudioAsset> AudioAsset("GameAssetFiles/Final Boss - Myuu.wav");
    GameAudio::PlaySound(AudioAsset);

    WindowManager WManager = WindowManager(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    GameManager GManager;

    GameEntity* Entity = CreateEntity<GameEntity>();
    BasicRendererComponent* Component = CreateComponent<BasicRendererComponent>(Entity);

    GManager.MainGameLoop();

    return 0;
} 
