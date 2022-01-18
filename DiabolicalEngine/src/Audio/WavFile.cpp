#include "WavFile.h"

DWAVFile::~DWAVFile()
{
	LOG_ERR("Deleted");
	if (AudioData)
	{
		delete (AudioData);
	}
}

std::shared_ptr<DWAVFile> DWAVFile::Load(DString filename)
{
	std::shared_ptr<DWAVFile> OutWav = std::make_shared<DWAVFile>();

	char buffer[4];

	std::ifstream in(*filename, std::ifstream::binary);
	in.read(buffer, 4);

	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		LOG_ERR("Not a valid WAV file, RIFF not found in header");
		return nullptr;
	}

	in.read(buffer, 4);//size of file. Not used. Read it to skip over it.  

	in.read(buffer, 4);//Format, should be WAVE

	if (strncmp(buffer, "WAVE", 4) != 0)
	{
		LOG_ERR("Not a valid WAV file, WAVE not found in header");
		return nullptr;
	}

	in.read(buffer, 4);//Format Space Marker. should equal fmt (space)

	if (strncmp(buffer, "fmt ", 4) != 0)
	{
		LOG_ERR("Not a valid WAV file, Format Marker not found in header");
		return nullptr;
	}

	in.read(buffer, 4);//Length of format data. Should be 16 for PCM, meaning uncompressed.

	if (AudioUtility::ConvertToInt(buffer, 4) != 16)
	{
		LOG_ERR("Not a valid WAV file, format length wrong in header");
		return nullptr;
	}

	in.read(buffer, 2);//Type of format, 1 = PCM

	if (AudioUtility::ConvertToInt(buffer, 2) != 1)
	{
		LOG_ERR("Not a valid WAV file, file not in PCM format");
		return nullptr;
	}

	in.read(buffer, 2);//Get number of channels. 

	//Assume at this point that we are dealing with a WAV file. This value is needed by OpenAL
	OutWav->channels = AudioUtility::ConvertToInt(buffer, 2);

	in.read(buffer, 4);//Get sampler rate. 

	OutWav->sampleRate = AudioUtility::ConvertToInt(buffer, 4);

	//Skip Byte Rate and Block Align. Maybe use later?
	in.read(buffer, 4);//Block Align
	in.read(buffer, 2);//ByteRate

	in.read(buffer, 2);//Get Bits Per Sample

	OutWav->bps = AudioUtility::ConvertToInt(buffer, 2);

	//Skip character data, which marks the start of the data that we care about. 
	in.read(buffer, 4);//"data" chunk. 

	in.read(buffer, 4); //Get size of the data

	OutWav->size = AudioUtility::ConvertToInt(buffer, 4);

	if (OutWav->size < 0)
	{
		LOG_ERR("Not a valid WAV file, size of file reports 0");
		return nullptr;
	}

	char* data = new char[OutWav->size];

	in.read(data, OutWav->size);//Read audio data into buffer, return.

	in.close();

	OutWav->AudioData = data;

	LOG(DString(OutWav->size));

	return OutWav;
}

char* DWAVFile::GetData()
{
	return AudioData;
}

int DWAVFile::GetChannels()
{
	return channels;
}

int DWAVFile::GetSampleRate()
{
	return sampleRate;
}

int DWAVFile::GetBitsPerSample()
{
	return bps;
}

int DWAVFile::GetSize()
{
	return size;
}