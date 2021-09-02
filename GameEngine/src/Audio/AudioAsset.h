#pragma once
#include "../Assets/GameAsset.h"

struct Mix_Chunk;


class AudioAsset : public GameAsset
{
public:

	static AudioAsset* TryLoad(std::filesystem::path Path);

	Mix_Chunk* GetMixChunk();
	
private:
	Mix_Chunk* MixChunk;
	float AudioDuration = 0.f;
};