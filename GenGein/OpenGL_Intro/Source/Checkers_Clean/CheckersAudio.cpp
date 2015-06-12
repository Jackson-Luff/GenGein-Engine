#include <iostream>
#include <FMOD\fmod_errors.h>

#include "CheckersAudio.h"

CheckersAudio::~CheckersAudio()
{
	m_pPianoSound->release();
	m_pFireSound->release();

	m_pFmodSystem->close();
	m_pFmodSystem->release();

	delete m_pFmodSystem;
	delete m_pFmodChannel;
	delete m_pFmodChannelGroup;

	delete m_pPianoSound;
	delete m_pFireSound;
}

void CheckersAudio::Initialise()
{
	if (!CreateFmodSystem())
	{
		printf("Failed to build FMOD system.\n");
		return;
	}

	CreateChannelGroup("CheckersSound");

	Create2DSound("Data/Audio/debussy.mp3", m_pPianoSound);
	Create2DSound("Data/Audio/fireplace.wav", m_pFireSound);

	PlaySounds();
}

void CheckersAudio::Update(const double a_dt)
{
	m_pFmodSystem->update();
}

// Private Func's

bool CheckersAudio::CreateFmodSystem()
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

bool CheckersAudio::CreateChannelGroup(const char* a_name)
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

bool CheckersAudio::Create2DSound(const char* a_dir, FMOD::Sound* a_sound, const uint32_t a_fmodType)
{
	/*
	/ FMOD_DEFAULT		- loads data straight into memory.
	/ FMOD_CREATESTREAM - streams in decoded whilst playing.
	*/

	FMOD_RESULT result;

	result = m_pFmodSystem->createSound(a_dir, a_fmodType | FMOD_2D, 0, &a_sound);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

bool CheckersAudio::Create3DSound(const char* a_dir, FMOD::Sound* a_sound, const FMOD_VECTOR a_pos, const uint32_t a_fmodType)
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

	result = m_pFmodSystem->createSound(a_dir, a_fmodType | FMOD_3D, 0, &a_sound);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s \n", result, FMOD_ErrorString(result));
		return false;
	}

	result = m_pFmodChannel->set3DAttributes(&a_pos, &velocity, 0);
	return true;
}

void CheckersAudio::PlaySounds()
{
	m_pFmodSystem->playSound(m_pPianoSound, m_pFmodChannelGroup, false, &m_pFmodChannel);
	m_pFmodSystem->playSound(m_pFireSound, m_pFmodChannelGroup, false, &m_pFmodChannel);
}