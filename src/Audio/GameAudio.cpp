#include "GameAudio.h"
#include "../CoreEngine.h"
#include <SDL2/SDL_mixer.h>
#include "AudioAsset.h"

void GameAudio::PlaySound(GameAssetSoftPointer<AudioAsset>& AudioAssetSoftPointer, float Volume)
{
    Check(AudioAssetSoftPointer.GetFileExtension() == ".wav");
    int Frequency = 22050;
    Uint16 Format = AUDIO_S16SYS;
    int Canal = 2; // 1 mono; 2 = stereo;
    int Buffer = 4096;
    Mix_OpenAudio(Frequency, Format, Canal, Buffer);


    if (!AudioAssetSoftPointer.Get())
    {
        AudioAssetSoftPointer.LoadSynchronous();
    }
    Check(AudioAssetSoftPointer.Get()); // make sure its actually loaded :P

    Mix_Chunk* Sound = AudioAssetSoftPointer.Get()->GetMixChunk();

    Sound->volume = (uint8_t)(128.f * Volume);

    Mix_PlayChannel(-1, Sound, 0);
}