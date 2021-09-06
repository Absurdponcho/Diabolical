#pragma once
#undef PlaySound
#include "AudioAsset.h"
#include "../Assets/GameAssetSoftPointer.h"

// Audio channel priority system. Oldest sounds of the lowest 
// priority will be overwritten first. Higher priority sounds 
// are never overwritten by lower priorities until they are complete

#define AUDIO_CHANNEL_NUM 8
#define AUDIO_CHANNEL_HIGHEST_PRIORITY 100000000
#define AUDIO_SAMPLE_RATE 44100

class AudioAsset;

// TODO:: Manage audio channels so that newer sounds stop oldest sounds from playing
// Also todo: need audio priority system

struct AudioChannel
{
public:
	int Priority = -AUDIO_CHANNEL_HIGHEST_PRIORITY;
	float TimePlayed = -10000000000.f;
};

class GameAudio
{
public:
	static int PlaySound(GameAssetSoftPointer<AudioAsset>& AudioAssetSoftPointer, float Volume = 1.f, int AudioPriority = 0, bool bLooping = false);
private:
	static int GetChannel(int MaximumPriority);
	static int GetLowestPriorityChannel(int MaximumPriority);
	static int GetOldestChannelOfPriority(int Priority);
	static AudioChannel AudioChannels[8];
};