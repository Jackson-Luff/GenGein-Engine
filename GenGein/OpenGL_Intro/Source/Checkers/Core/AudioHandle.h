#pragma once
#include <FMOD\fmod.hpp>
#include <stdint.h>

////
// Author: Jackson Luff
// Name: AudioHandle
// Type: class (standard)
// Parent: none
// Description:
// * This class handles and
// * wraps a lot of the content
// * needed and used in FMOD
// * for all audio purposes 
// * in this demo.
// NOTE: WILL BE A THING IN THE ENGINE SOON
////
class AudioHandle
{
public:
	// Deconstructor
	~AudioHandle();

	// Initialises FMOD system, channel and sounds
	void Initialise();
	//
	void Shutdown();
	// Updates FMOD's audio stream
	void Update(const double a_dt);
	// This plays the sliding effect
	void PlayMoveSound();
	// This plays the click after being placed
	void PlayHasMovedSound();

private:
	// Builds the FMOD system (build before ANYTHING)
	bool CreateFmodSystem();
	// Builds the channel group for audio compontent
	bool CreateChannelGroup(const char* a_name);
	// Creates a 2D sound (must make channel group prior)
	bool Create2DSound(const char* a_dir, FMOD::Sound& a_sound, const uint32_t a_fmodType = FMOD_DEFAULT);
	// Creates a 3D sound (must make channel group prior)
	bool Create3DSound(const char* a_dir, FMOD::Sound& a_sound, const FMOD_VECTOR a_pos, const uint32_t a_fmodType = FMOD_DEFAULT);
	// Plays the background sounds on intialise
	void PlayBackgroundSounds();

	//FMOD manager compontents
	FMOD::System* m_pFmodSystem;
	FMOD::Channel* m_pBackgroundChannel;
	FMOD::Channel* m_pEffectsChannel;
	FMOD::ChannelGroup* m_pFmodChannelGroup;

	//FMOD input audio
	FMOD::Sound* m_pPianoSound;
	FMOD::Sound* m_pFireSound;
	FMOD::Sound* m_pMoveSound;
	FMOD::Sound* m_pSlideSound;
};