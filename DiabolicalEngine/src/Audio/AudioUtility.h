#pragma once
#include <string>
#include <iostream>
#include "../Logging/Logging.h"
#include <fstream>
#include <AL/al.h>
#include "../Types/DString.h"

class DAudioFile
{
public:
	virtual ~DAudioFile(){}
	
	virtual char* GetData() = 0;
	virtual int GetChannels() = 0;
	virtual int GetSampleRate() = 0;
	virtual int GetBitsPerSample() = 0;
	virtual int GetSize() = 0;
};

class AudioUtility
{
public:
	static ALenum ToAlFormat(short channels, short samples)
	{
		bool stereo = (channels > 1);

		switch (samples) {
		case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
		case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
		default:
		return -1;
		}
	}

	static bool IsBigEndian(void)
	{
		int a = 1;
		return !((char*)&a)[0];
	}

	static int ConvertToInt(char* buffer, int len)
	{
		int a = 0;

		if (!IsBigEndian())
		{
			for (int i = 0; i < len; ++i)
			{
				((char*)&a)[i] = buffer[i];
			}
		}
		else
		{
			for (int i = 0; i < len; ++i)
			{
				((char*)&a)[3 - i] = buffer[i];
			}
		}

		return a;
	}
};



