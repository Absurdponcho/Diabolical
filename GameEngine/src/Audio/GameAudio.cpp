#include "GameAudio.h"
#include "../CoreEngine.h"
#include <SDL2/SDL_mixer.h>
#include "AudioAsset.h"


AudioChannel GameAudio::AudioChannels[8]{};

int GameAudio::PlaySound(GameAssetSoftPointer<AudioAsset>& AudioAssetSoftPointer, float Volume, int AudioPriority, bool bLooping)
{
    Check(AudioAssetSoftPointer.GetFileExtension() == ".wav");
    static bool bInitialized = false;
    if (!bInitialized)
    {
        int Frequency = AUDIO_SAMPLE_RATE;
        Uint16 Format = AUDIO_S16SYS;
        int Channel = 2; // 1 mono; 2 = stereo;
        int Buffer = 4096;
        Mix_OpenAudio(Frequency, Format, Channel, Buffer);
        bInitialized = true;
    }

    if (!AudioAssetSoftPointer.Get())
    {
        AudioAssetSoftPointer.LoadSynchronous();
    }
    Check(AudioAssetSoftPointer.Get()); // make sure its actually loaded :P

    Mix_Chunk* Sound = AudioAssetSoftPointer.Get()->GetMixChunk();

    Sound->volume = (uint8_t)(128.f * Volume);

    int FreeChannel = GetChannel(AudioPriority);
    LOGVERBOSE("GameAudio::PlaySound()", "Playing sound on channel " + std::to_string(FreeChannel));
    return Mix_PlayChannel(FreeChannel, Sound, bLooping ? -1 : 0);
}

int GameAudio::GetChannel(int MaximumPriority)
{
    int LowestPriority = GetLowestPriorityChannel(MaximumPriority);
    int Oldest = GetOldestChannelOfPriority(AudioChannels[LowestPriority].Priority);
    return Oldest;
}

int GameAudio::GetLowestPriorityChannel(int MaximumPriority)
{

    int LowestPriority = AudioChannels[0].Priority;
    int LowestPriorityChannel = 0;
    for (int Index = 0; Index < AUDIO_CHANNEL_NUM; Index++)
    {
        AudioChannel& Channel = AudioChannels[Index];
        if (Channel.Priority < LowestPriority || Mix_Playing(Index) == 0)
        {
            LowestPriority = Channel.Priority;
            LowestPriorityChannel = Index;
        }
    }
    if (LowestPriorityChannel >= 0)
    {
        
        AudioChannels[LowestPriorityChannel].Priority = MaximumPriority;
    }

    return LowestPriorityChannel;
}


int GameAudio::GetOldestChannelOfPriority(int Priority)
{
    int OldestChannel = 0;
    float Time = GameManager::GetTime();
    for (int Index = 0; Index < AUDIO_CHANNEL_NUM; Index++)
    {
        AudioChannel& Channel = AudioChannels[Index];
        if ((Channel.TimePlayed < Time && Channel.Priority == Priority) || Mix_Playing(Index) == 0)
        {
            Time = Channel.TimePlayed;
            OldestChannel = Index;
        }
    }
    if (OldestChannel >= 0) {
        AudioChannels[OldestChannel].TimePlayed = GameManager::GetTime();

    }

    return OldestChannel;
}