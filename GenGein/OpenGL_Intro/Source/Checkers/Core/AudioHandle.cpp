#include <iostream>
#include <FMOD\fmod_errors.h>

#include "AudioHandle.h"

AudioHandle::~AudioHandle()
{}

void AudioHandle::Initialise()
{
	if (!CreateFmodSystem())
	{
		printf("Failed to build FMOD system.\n");
		return;
	}

	if (!CreateChannelGroup("CheckersSound"))
		return;

	m_pPianoSound = nullptr;
	m_pFireSound = nullptr;
	m_pSlideSound = nullptr;
	m_pMoveSound = nullptr;

	Create2DSound("Data/Audio/debussy.mp3", &m_pPianoSound, FMOD_LOOP_NORMAL);
	Create2DSound("Data/Audio/fireplace.wav", &m_pFireSound, FMOD_LOOP_NORMAL);
	Create2DSound("Data/Audio/slide.wav", &m_pSlideSound, FMOD_DEFAULT);
	Create2DSound("Data/Audio/click.wav", &m_pMoveSound, FMOD_DEFAULT);

	//Create2DSound("Data/Audio/debussy.mp3", *m_pPianoSound);
	//Create2DSound("Data/Audio/fireplace.wav", *m_pFireSound);

	PlayBackgroundSounds();
}

void AudioHandle::Shutdown()
{
	m_pPianoSound->release();
	m_pFireSound->release();
	m_pSlideSound->release();
	m_pMoveSound->release();

	m_pFmodSystem->close();
	m_pFmodSystem->release();
}

void AudioHandle::Update(const double a_dt)
{
	m_pFmodSystem->update();
}

void AudioHandle::PlayMoveSound()
{
	bool isActive;
	if (m_pEffectsChannel->isPlaying(&isActive) == FMOD_OK && isActive)
		m_pEffectsChannel->stop();

	m_pFmodSystem->playSound(m_pSlideSound, m_pFmodChannelGroup, false, &m_pEffectsChannel);
	m_pEffectsChannel->setVolume(1);
}

void AudioHandle::PlayHasMovedSound()
{
	bool isActive;
	if (m_pEffectsChannel->isPlaying(&isActive) == FMOD_OK && isActive)
		m_pEffectsChannel->stop();

	m_pFmodSystem->playSound(m_pMoveSound, m_pFmodChannelGroup, false, &m_pEffectsChannel);
}

// Private Func's

bool AudioHandle::CreateFmodSystem()
{
	// Initialise FMOD
	FMOD_RESULT result;
	m_pFmodSystem = NULL;

	// Create the main system object.
	result = FMOD::System_Create(&m_pFmodSystem);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}

	// Initialise FMOD with 512 channels
	result = m_pFmodSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}

	return true;
}

bool AudioHandle::CreateChannelGroup(const char* a_name)
{
	FMOD_RESULT result;
	result = m_pFmodSystem->createChannelGroup(a_name, &m_pFmodChannelGroup);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

bool AudioHandle::Create2DSound(const char* a_dir, FMOD::Sound** a_sound, const uint32_t a_fmodType)
{
	/*
	/ FMOD_DEFAULT		- loads data straight into memory.
	/ FMOD_CREATESTREAM - streams in decoded whilst playing.
	*/

	FMOD_RESULT result;
	result = m_pFmodSystem->createSound(a_dir, a_fmodType | FMOD_2D, 0, a_sound);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

bool AudioHandle::Create3DSound(const char* a_dir, FMOD::Sound** a_sound, const FMOD_VECTOR a_pos, const uint32_t a_fmodType)
{
	/*
	/ FMOD_DEFAULT		- loads data straight into memory.
	/ FMOD_CREATESTREAM - streams in decoded whilst playing.
	*/

	FMOD_VECTOR velocity = { 0, 0, 0 };
	FMOD_VECTOR forward = { 1, 0, 0 };
	FMOD_VECTOR up = { 0, 1, 0 };

	m_pFmodSystem->set3DListenerAttributes(0, &a_pos, &velocity, &forward, &up);

	FMOD_RESULT result;

	result = m_pFmodSystem->createSound(a_dir, a_fmodType | FMOD_3D, 0, a_sound);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

void AudioHandle::PlayBackgroundSounds()
{
	m_pFmodSystem->playSound(m_pPianoSound, m_pFmodChannelGroup, false, &m_pBackgroundChannel);
	m_pFmodSystem->playSound(m_pFireSound, m_pFmodChannelGroup, false, &m_pBackgroundChannel);
}