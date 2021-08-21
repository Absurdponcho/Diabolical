#include "GameAudio.h"
#include "../CoreEngine.h"
#include <SDL2/SDL_mixer.h>

void GameAudio::PlaySound(GameAssetSoftPointer& AudioAssetSoftPointer, float Volume)
{
    Check(AudioAssetSoftPointer.GetFileExtension() == ".wav");
    int Frequency = 22050;
    Uint16 Format = AUDIO_S16SYS;
    int Canal = 2; // 1 mono; 2 = stereo;
    int Buffer = 4096;
    Mix_OpenAudio(Frequency, Format, Canal, Buffer);

    Mix_Chunk* Sound;

    Sound = Mix_LoadWAV((char*)AudioAssetSoftPointer.GetPath().u8string().c_str());

    Sound->volume = (uint8_t)(128.f * Volume);

    Mix_PlayChannel(-1, Sound, 0);
}