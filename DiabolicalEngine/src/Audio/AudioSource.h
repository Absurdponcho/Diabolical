#pragma once
#include "AudioUtility.h"
#include "Types/DMemory.h"

class DAudioSource
{
public:
	~DAudioSource();
	void SetAudioFile(const DSharedPtr<DAudioFile>& NewAudioFile);
	void Play();

private:
	DSharedPtr<DAudioFile> AudioFile;
};

