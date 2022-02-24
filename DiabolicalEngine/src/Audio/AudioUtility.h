#pragma once
#include <string>
#include <iostream>
#include "Logging/Logging.h"
#include <fstream>
#include <AL/al.h>
#include "Types/DString.h"

class DAudioFile
{
public:
	virtual ~DAudioFile(){}
	
	virtual const char* GetData() const = 0;
	virtual int GetChannels() const = 0;
	virtual int GetSampleRate() const = 0;
	virtual int GetBitsPerSample() const = 0;
	virtual int GetSize() const = 0;
	virtual bool IsValid() const = 0;
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
		Check(false);
		return -1;
		}
	}


	static int ConvertToInt(const char* buffer, int len, bool BigEndian = false)
	{
		int a = 0;

		if (!BigEndian)
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
				((char*)&a)[(len-1) - i] = buffer[i];
			}
		}

		return a;
	}
};



