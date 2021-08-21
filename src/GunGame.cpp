#include "CoreEngine.h"
#undef main


int main(int argc, char** argv)
{
    Logging::SetLogVerbosity(ELogVerbosity::LV_Verbose);

    WindowManager WManager = WindowManager(
        "GunGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    GameAssetSoftPointer<AudioAsset> AudioAsset("GameAssetFiles/Final Boss - Myuu.wav");
    GameAudio::PlaySound(AudioAsset);

    GameManager GManager;
    GManager.MainGameLoop();

    return 0;
} 
