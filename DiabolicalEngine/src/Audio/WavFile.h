#pragma once
#include "AudioUtility.h"
#include <AL/AL.h>
#include <AL/ALC.h>

class DWAVFile : public DAudioFile
{
public:
	virtual ~DWAVFile();

	static std::shared_ptr<DWAVFile> Load(DString filename);

	virtual char* GetData() override;
	virtual int GetChannels() override;
	virtual int GetSampleRate() override;
	virtual int GetBitsPerSample() override;
	virtual int GetSize() override;

private:
	int channels = -1;
	int sampleRate = -1;
	int bps = -1;
	int size = -1;
	char* AudioData = nullptr;
};
