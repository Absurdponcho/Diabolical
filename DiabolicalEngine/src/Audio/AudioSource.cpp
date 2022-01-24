#include "AudioSource.h"
#include <AL/AL.h>
#include <AL/ALC.h>

/*AudioSource = new DAudioSource();
{
	std::shared_ptr<DWAVFile> WavFile = DWAVFile::Load("Assets/Sussy Baka.wav");
	AudioSource->SetAudioFile(WavFile);
	AudioSource->Play();
}*/

DAudioSource::~DAudioSource()
{
	LOG("Audio source destroyed");
	
}

void DAudioSource::SetAudioFile(std::shared_ptr<DAudioFile> NewAudioFile)
{
	AudioFile = NewAudioFile;
	
}

void DAudioSource::Play()
{
	Check(AudioFile->GetData());
	if (!AudioFile->GetData())
		return;

	ALCenum error;

	ALCdevice* device = alcOpenDevice(nullptr);
	Check(device);
	ALCcontext* context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
	{
		Check(false);
	}

	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);

	ALuint source;
	alGenSources((ALuint)1, &source);
	error = alGetError();
	Check(error == AL_NO_ERROR);

	alSourcef(source, AL_PITCH, 1);
	error = alGetError();
	Check(error == AL_NO_ERROR);
	alSourcef(source, AL_GAIN, 1);
	error = alGetError();
	Check(error == AL_NO_ERROR);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	error = alGetError();
	Check(error == AL_NO_ERROR);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	error = alGetError();
	Check(error == AL_NO_ERROR);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	error = alGetError();
	Check(error == AL_NO_ERROR);

	ALuint buffer;

	alGenBuffers((ALuint)1, &buffer);
	error = alGetError();
	Check(error == AL_NO_ERROR);

	ALenum Format = AudioUtility::ToAlFormat(AudioFile->GetChannels(), AudioFile->GetBitsPerSample());

	alBufferData(buffer, Format, AudioFile->GetData(), AudioFile->GetSize(), AudioFile->GetSampleRate());
	error = alGetError();
	Check(error == AL_NO_ERROR);

	alSourcei(source, AL_BUFFER, buffer);

	alSourcePlay(source);
	error = alGetError();
	Check(error == AL_NO_ERROR);
}

