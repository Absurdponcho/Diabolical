#include "Logging/Logging.h"
#include "WindowManager.h"
#include "ECS/GameEntity.h"
#include "GameManager.h"
#include "Assets/GameAssetSoftPointer.h"
#include "Audio/GameAudio.h"
#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);

    WindowManager WManager = WindowManager(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    GameAssetSoftPointer AudioAsset("GameAssetFiles/phaser1.wav");

    GameAudio::PlaySound(AudioAsset);


    GameManager GManager;
    GManager.MainGameLoop();

    return 0;
} 
