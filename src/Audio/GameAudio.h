#pragma once
#undef PlaySound

class GameAssetSoftPointer;

// TODO:: Manage audio channels so that newer sounds stop oldest sounds from playing
// Also todo: need audio priority system

class GameAudio
{
public:
	static void PlaySound(GameAssetSoftPointer& AudioAssetSoftPointer, float Volume = 1.f);
};