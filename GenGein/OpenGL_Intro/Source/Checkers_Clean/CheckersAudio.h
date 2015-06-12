#pragma once
#include <FMOD\fmod.hpp>
#include <stdint.h>

class CheckersAudio
{
public:
	~CheckersAudio();

	void Initialise();
	void Update(const double a_dt);

private:
	bool CreateFmodSystem();
	bool CreateChannelGroup(const char* a_name);
	bool Create2DSound(const char* a_dir, FMOD::Sound* a_sound, const uint32_t a_fmodType = FMOD_DEFAULT);
	bool Create3DSound(const char* a_dir, FMOD::Sound* a_sound, const FMOD_VECTOR a_pos, const uint32_t a_fmodType = FMOD_DEFAULT);
	void PlaySounds();
	
	FMOD::System* m_pFmodSystem;
	FMOD::Channel* m_pFmodChannel;
	FMOD::ChannelGroup* m_pFmodChannelGroup;
	
	FMOD::Sound* m_pPianoSound;
	FMOD::Sound* m_pFireSound;
};