#pragma once
#undef PlaySound
#include "AudioAsset.h"
#include "../Assets/GameAssetSoftPointer.h"

class AudioAsset;

// TODO:: Manage audio channels so that newer sounds stop oldest sounds from playing
// Also todo: need audio priority system

class GameAudio
{
public:
	static void PlaySound(GameAssetSoftPointer<AudioAsset>& AudioAssetSoftPointer, float Volume = 1.f);
};