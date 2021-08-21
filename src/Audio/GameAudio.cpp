#include "GameAudio.h"
#include <SDL2/SDL_mixer.h>
#include "../Assets/GameAssetSoftPointer.h"

void GameAudio::PlaySound(GameAssetSoftPointer& AudioAssetSoftPointer)
{
    int frequencia = 22050;
    Uint16 formato = AUDIO_S16SYS;
    int canal = 2; // 1 mono; 2 = stereo;
    int buffer = 4096;
    Mix_OpenAudio(frequencia, formato, canal, buffer);

    Mix_Chunk* sound_1;

    sound_1 = Mix_LoadWAV((char*)AudioAssetSoftPointer.GetPath().u8string().c_str());

    Mix_PlayChannel(-1, sound_1, 0);
}