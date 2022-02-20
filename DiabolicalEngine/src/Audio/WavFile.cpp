#include "WavFile.h"
#include "../AssetManager/AssetManager.h"

DWAVFile::~DWAVFile()
{
	LOG_ERR("Deleted");
}

void DWAVFile::LoadAsync(DString filename, AAsyncWAVLoad OnLoad)
{
	DAssetManager::Get().AsyncLoadAsset(filename, [=](std::shared_ptr<DRawAsset> Asset) mutable
	{
		Check(Asset.get());
		Check(Asset->IsValid());
		if (Asset.get() && Asset->IsValid())
		{
			std::shared_ptr<DWAVFile> NewDWav = DWAVFile::Load(Asset);
			Check (NewDWav.get());
			if (OnLoad.IsBound() && NewDWav.get())
			{
				OnLoad.Invoke(NewDWav);
			}
		}
	});
}

std::shared_ptr<DWAVFile> DWAVFile::Load(std::shared_ptr<DRawAsset> Asset)
{
	Check(Asset.get());
	Check(Asset->IsValid());
	if (!Asset.get() || !Asset->IsValid())
	{
		LOG_ERR("!Asset.get() || !Asset->IsValid()");
		return nullptr;
	}
	const char* buffer = Asset->GetData<char>();

	Check(buffer);
	if (!buffer)
	{
		LOG_ERR("Invalid buffer");
		return nullptr;
	}

	WavHeader& Header = *(WavHeader*)buffer;

	if (strncmp(Header.ChunkID, "RIFF", 4) != 0)
	{
		LOG_ERR("Not a valid WAV file, RIFF not found in header");
		return nullptr;
	}

	if (strncmp(Header.Format, "WAVE", 4) != 0)
	{
		LOG_ERR("Not a valid WAV file, WAVE not found in header");
		return nullptr;
	}

	if (strncmp(Header.Subchunk1ID, "fmt ", 4) != 0)
	{
		LOG_ERR("Not a valid WAV file, Format Marker not found in header");
		return nullptr;
	}

	if (Header.Subchunk1Size != 16)
	{
		LOG_ERR("Not a valid WAV file, format length wrong in header");
		return nullptr;
	}

	if (Header.AudioFormat != 1)
	{
		LOG_ERR("Not a valid WAV file, file not in PCM format");
		return nullptr;
	}

	std::shared_ptr<DWAVFile> OutWav = std::make_shared<DWAVFile>(Asset);

	OutWav->channels = Header.NumChannels;
	OutWav->sampleRate = Header.SampleRate;
	OutWav->bps = Header.BitsPerSample;
	OutWav->size = Header.Subchunk2Size;

	if (OutWav->size < 0)
	{
		LOG_ERR("Not a valid WAV file, size of file reports 0");
		return nullptr;
	}

	OutWav->AudioData = &Header.Data;

	LOG(DString(OutWav->size));

	return OutWav;
}

const char* DWAVFile::GetData() const 
{
	return AudioData;
}

int DWAVFile::GetChannels() const 
{
	return channels;
}

int DWAVFile::GetSampleRate() const 
{
	return sampleRate;
}

int DWAVFile::GetBitsPerSample() const 
{
	return bps;
}

int DWAVFile::GetSize() const 
{
	return size;
}

bool DWAVFile::IsValid() const
{
	return AssetRef.get() && AssetRef->IsValid();
}
