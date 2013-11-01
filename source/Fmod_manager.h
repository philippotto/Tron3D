//////////////////////////////////////////////////////////////////////////
// Author:              Dan Polak
// Date:                10/31/2009
// Last Mod:    10/31/2009 3:10pm
//
// File:                FMOD Manager.h
//
// Purpose:     Declare the cFMODManager class along with any members and 
//                      functions it will use. This class will handle playing, stoping,
//                      re-setting, and assigning channels for all game sounds.
//////////////////////////////////////////////////////////////////////////

#pragma once
//#include "fmod.h"
#pragma comment(lib, "fmodex64_vc.lib")

//#include "vec3.h"
#include <QVector3D>

//Access
#include <vector>
using std::vector;
#include "fmod.hpp"
#include "fmod_errors.h"


//#pragma comment(lib, "fmodex64_vc.lib")

#define MAX_CHANNELS 50

//Sound Indices
//BG MUSIC indices in both playing and stored sounds will be the same
#define BG_MUSIC_BASE           0
#define BG_MUSIC_BATTLE         1
#define BG_MUSIC_VILLIAN        2

#define BG_MUSIC_LOSS           3
#define BG_MUSIC_WIN            4

#define MIN_FOOTSTEP            5
#define MAX_FOOTSTEP            14

#define MELEE_HIT               15



class cFMODManager
{
private:
public:
	float m_fMusicVolume;
	float m_fSFXVolume;

	bool m_bFading;
	char m_cDestinationFade;
	char m_cSourceFade;

	//Are we fading into or out of the 

	//Used for calling, will set a variable for readability purposes when loaded in 
	unsigned int m_nSoundCount;
	//Used for passing ID's to stop sounds
	unsigned int m_nPlayedSounds;

	struct tGameSound
	{
		int m_GSoundID;
		int m_nSoundID;
		FMOD::Channel* pChannel;
		QVector3D m_vPosition;
	};

	struct tSound
	{
		FMOD::Sound* m_sound;
		std::string m_szFilename;
		int m_nType;
		float m_fVolume;
		bool m_bLooping;
	};

	//Possible sounds
	std::vector<tSound*> m_vSounds;
	std::vector<tGameSound*> m_vPlayingSounds;

	//Instance of Manager to keep it a singleton
	static cFMODManager* m_pInstance;

	//FMOD System
	FMOD::System* m_pSystem;

	//Place Trilogy here

public:
	//////////////////////////////////////////////////////////////////////////
	//      Function:       GetInstance
	//
	//      Purpose:        Returns an instance of the sound
	//                              manager for use throughout the game.
	//
	//      Paramaters:     N/A
	//////////////////////////////////////////////////////////////////////////
	static cFMODManager* GetInstance(void);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       InitFMOD
	//
	//      Purpose:        Initialze FMOD manager and Load in sounds
	//                              to be used throughout the game. Sets
	//                              any necessary flags for playing sounds.
	//                              Will return true upon success and false
	//                              if an error occurs.
	//
	//      Paramaters:     nMaxChannels    - Sets the maximum channels
	//                                                              of sound to be used.
	//
	//                              unFlags                 - Any flags that may need
	//                                                              to be set for playing sounds.
	//////////////////////////////////////////////////////////////////////////
	bool InitFMOD(int nMaxChannels, FMOD_INITFLAGS unFlags);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       LoadSound
	//
	//      Purpose:        Load in and store a sound for use throughout the game. This
	//                              Function will return an integer of it's index for access
	//                              throughout the game. Returns -1 if there was an error
	//                              loading the sound.
	//
	//      Paramaters:     szFileName              - The filename of the sound to be loaded.
	//                              bLooping                - Check if the sound is a looping one.
	//                              b3D                             - Check if the sound is a 3D one.
	//                              fVol                    - Volume of the sound.
	//                              uiType                  - Determine if the sound is a sound effect
	//                                                              or a background sound, 
	//////////////////////////////////////////////////////////////////////////
	unsigned char LoadSound(const char* szFileName, bool bLooping, bool b3D, float fVol);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       UnloadSound
	//
	//      Purpose:        Unload sounds according to the given ID. Releases sounds at
	//                              end of the program. Returns true upon success, false if
	//                              an error occurs.
	//
	//      Paramaters:     nID                             -The index of the file to be unloaded.
	//////////////////////////////////////////////////////////////////////////
	bool UnloadSound(int nID);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       PlaySound
	//
	//      Purpose:        Starts playing a sound, it will start at the beginning if
	//                              reset, or wherever the sound left off at.
	//
	//      Paramaters:     nID                             - The index of the sound to be played.
	//////////////////////////////////////////////////////////////////////////
	unsigned int PlayFMODSound(int nID);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       ResetSound
	//
	//      Purpose:        Resets a sound that was stopped or has finished. Returns 
	//                              true if the sound was reset correctly and false if an
	//                              error has occured.
	//
	//      Paramaters:     nID                             - Index of the sound to be reset.
	//////////////////////////////////////////////////////////////////////////
	bool ResetSound(int nID);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       StopSound
	//
	//      Purpose:        Stops a sound that is playing, but does not reset it.
	//                              Reset will have to be called seperately if that is the
	//                              intent.
	//
	//      Paramaters:     nID                             - Index of the sound to stop.
	//////////////////////////////////////////////////////////////////////////
	bool StopSound(int nID);

	//////////////////////////////////////////////////////////////////////////
	//      Function:       Update
	//
	//      Purpose:        Update the sound objects and their positions. Will reset
	//                              the information of any channels that are done.
	//
	//      Paramaters:     fDelta                  - Delta Time
	//////////////////////////////////////////////////////////////////////////
	bool Update(float fDelta);

};
