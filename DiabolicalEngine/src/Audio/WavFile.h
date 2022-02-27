#pragma once
#include "AudioUtility.h"
#include <AL/AL.h>
#include <AL/ALC.h>
#include "Types/Action.h"
#include "Types/DMemory.h"

typedef DAction<DSharedPtr<class DWAVFile>> AAsyncWAVLoad;

struct WavHeader
{
	char ChunkID[4];
	uint32_t ChunkSize;
	char Format[4];
	char Subchunk1ID[4];
	uint32_t Subchunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
	uint32_t Subchunk2ID;
	uint32_t Subchunk2Size;
	char Data;
};

class DWAVFile : public DAudioFile
{
public:
	virtual ~DWAVFile();

	DWAVFile() {}
	DWAVFile(DSharedPtr<class DRawAsset> Asset)
		: AssetRef(Asset)
	{}

	static void LoadAsync(DString filename, AAsyncWAVLoad OnLoad);

	virtual const char* GetData() const override;
	virtual int GetChannels() const override;
	virtual int GetSampleRate() const override;
	virtual int GetBitsPerSample() const override;
	virtual int GetSize() const override;
	virtual bool IsValid() const override;

protected:

	DSharedPtr<class DRawAsset> AssetRef;
private:
	static DSharedPtr<DWAVFile> Load(DSharedPtr<class DRawAsset> Asset);
	int channels = -1;
	int sampleRate = -1;
	int bps = -1;
	int size = -1;
	const char* AudioData = nullptr;
};
