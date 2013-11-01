#include "FMOD_manager.h"


cFMODManager* cFMODManager::m_pInstance = NULL;

cFMODManager* cFMODManager::GetInstance(void)
{
	if (!m_pInstance)
		m_pInstance = new cFMODManager();

	return m_pInstance;
}

bool cFMODManager::InitFMOD(int nMaxChannels, FMOD_INITFLAGS unFlags)
{
	//Create system
	FMOD::System_Create(&m_pSystem);

	//Set up Volumes
	this->m_fSFXVolume = 1.0f;
	this->m_fMusicVolume = 1.0f;
	this->m_vPlayingSounds.clear();
	this->m_vSounds.clear();

	//Fade settings
	this->m_bFading = false;
	this->m_cDestinationFade = BG_MUSIC_BASE;
	this->m_cSourceFade = BG_MUSIC_BASE;

	//Set up important members
	this->m_nSoundCount = -1;
	this->m_nPlayedSounds = 0;

	//Create a result for error checking
	FMOD_RESULT result;

	//Init and error check
	result = m_pSystem->init(50, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK)
	{
		return false;
	}

	return true;
}
unsigned char cFMODManager::LoadSound(const char* szFileName, bool bLooping, bool b3D, float fVol)
{
	//Check for a system to work with
	if (!m_pSystem)
	{
		return -1;
	}

	//Make sure we actually have a file
	if (!szFileName)
	{
		return -1;
	}

	//Not checking for duplicates let's not be dumb
	//Need this to make sure things went through well
	FMOD_RESULT result;

	//Create the new sound
	tSound* newSound = new tSound;

	//Fill in the information for the new sound
	newSound->m_szFilename = szFileName;
	newSound->m_fVolume = fVol;
	newSound->m_bLooping = bLooping;

	//Is sound 3D or not?
	if (b3D)
	{
		//Check for looping
		if (!bLooping)
		{
			result = m_pSystem->createStream(newSound->m_szFilename.c_str(), FMOD_3D, 0, &newSound->m_sound);
		}
		//It's looping
		else
		{
			result = m_pSystem->createStream(newSound->m_szFilename.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, 0, &newSound->m_sound);
		}
	}
	//Not 3D sound
	else
	{
		//Check for looping
		if (!bLooping)
		{
			result = m_pSystem->createStream(newSound->m_szFilename.c_str(), FMOD_2D, 0, &newSound->m_sound);
		}
		//It's looping
		else
		{
			result = m_pSystem->createStream(newSound->m_szFilename.c_str(), FMOD_2D | FMOD_LOOP_NORMAL, 0, &newSound->m_sound);
		}
	}

	m_nSoundCount += 1;

	this->m_vSounds.push_back(newSound);

	return m_nSoundCount;
}

bool cFMODManager::UnloadSound(int nID)
{

	return true;
}

unsigned int cFMODManager::PlayFMODSound(int nID)
{
	//Need a system to even play the sound
	if (!m_pSystem)
	{
		return 0;
	}

	//check to make sure we have a valid ID
	if (nID > 0)
	{
		if (nID > (int)m_nSoundCount)
			return 0;
	}

	//Incriment played sounds
	this->m_nPlayedSounds += 1;

	//Create New Game Sound
	tGameSound* gSound = new tGameSound();

	gSound->m_nSoundID = nID;
	gSound->m_nSoundID = this->m_nPlayedSounds;

	FMOD_RESULT result;

	//Test version
	result = m_pSystem->playSound(FMOD_CHANNEL_FREE, m_vSounds[nID]->m_sound, true, &gSound->pChannel);

	if (result != FMOD_OK)
		return false;

	//Set Volume and unpause sound
	result = gSound->pChannel->setVolume(m_vSounds[nID]->m_fVolume);
	if (result != FMOD_OK)
		return false;

	result = gSound->pChannel->setPaused(false);
	if (result != FMOD_OK)
		return false;

	m_vPlayingSounds.push_back(gSound);

	return this->m_nPlayedSounds;
}

bool cFMODManager::ResetSound(int nID)
{
	m_vPlayingSounds[nID]->pChannel->setPosition(0, FMOD_TIMEUNIT_MS);

	return true;
}

bool cFMODManager::StopSound(int nID)
{
	int index = -1;
	for (int i = 0; i < m_vPlayingSounds.size(); ++i)
	{
		if (m_vPlayingSounds[i]->m_GSoundID == nID)
		{
			//Set the index to work with
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		return false;
	}

	bool b;
	this->m_vPlayingSounds[nID]->pChannel->isPlaying(&b);

	if (b)
	{
		FMOD_RESULT result;
		result = this->m_vPlayingSounds[nID]->pChannel->stop();

		//remove the given sound from the vector, it's done!
		m_vPlayingSounds.erase(m_vPlayingSounds.begin() + index);

		if (result != FMOD_OK)
			return false;
	}

	return true;
}

bool cFMODManager::Update(float fDelta)
{
	bool bCheck = false;

	//Update the FMOD System
	this->m_pSystem->update();

	//Handle any sounds that have stopped
	for (unsigned int i = 0; i < this->m_vPlayingSounds.size(); ++i)
	{
		m_vPlayingSounds[i]->pChannel->isPlaying(&bCheck);
		if (!bCheck)
		{
			m_vPlayingSounds.erase(m_vPlayingSounds.begin() + i);

			//Stay at the same index since we removed one!
			i -= 1;
		}
	}

	//Handle the this->m_nPlayedSounds, we only have 50 channels
	// so if the count is greater than 50 we can reset to 3 because
	// the first 3 sounds should be the background music.
	if (this->m_nPlayedSounds > 50)
	{
		this->m_nPlayedSounds = 3;
	}

	//Handling the bacground music fades
	if (this->m_bFading)
	{
		float fVol1, fVol2;
		m_vPlayingSounds[m_cSourceFade]->pChannel->getVolume(&fVol1);
		m_vPlayingSounds[m_cDestinationFade]->pChannel->getVolume(&fVol2);

		fVol1 -= 0.000001f;
		fVol2 += 0.000001f;

		if (fVol1 <= 0.0f || fVol2 >= 1.0f)
		{
			m_vPlayingSounds[m_cSourceFade]->pChannel->setVolume(0.0f);
			m_vPlayingSounds[m_cDestinationFade]->pChannel->setVolume(m_fMusicVolume);

			//No Longer fading
			m_bFading = false;
			//Set source to destination
			m_cSourceFade = m_cDestinationFade;
		}

		//Set Volumes not done fading
		else
		{
			m_vPlayingSounds[m_cSourceFade]->pChannel->setVolume(fVol1);
			m_vPlayingSounds[m_cDestinationFade]->pChannel->setVolume(fVol2);
		}

	}

	return true;
}
