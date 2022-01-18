#pragma once
#include "AudioUtility.h"

class DAudioSource
{
public:
	~DAudioSource();
	void SetAudioFile(std::shared_ptr<DAudioFile> NewAudioFile);
	void Play();

private:
	std::shared_ptr<DAudioFile> AudioFile;
};

