#include "AudioManager.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace troen::sound;

float ChangeSemitone(float frequency, float variation) {
	static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);
	return frequency * pow(semitone_ratio, variation);
}

float RandomBetween(float min, float max) {
	if(min == max) return min;
	float n = (float)rand()/(float)RAND_MAX;
	return min + n * (max - min);
}

AudioManager::AudioManager() : currentSong(0), fade(FADE_NONE) {
	// Initialize system
	FMOD::System_Create(&system);
	system->init(100, FMOD_INIT_NORMAL, 0);
	
	// Create channels groups for each category
	system->getMasterChannelGroup(&master);
	for(int i = 0; i < CATEGORY_COUNT; ++i) {
		system->createChannelGroup(0, &groups[i]);
		master->addGroup(groups[i]);
	}
	// Set up modes for each category
	modes[CATEGORY_SFX] = FMOD_DEFAULT;
	modes[CATEGORY_SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;
	
	// Seed random number generator for SFXs
	srand(time(0));
}

AudioManager::~AudioManager() {
	// Release sounds in each category
	SoundMap::iterator iter;
	for(int i = 0; i < CATEGORY_COUNT; ++i) {
		for (iter = sounds[i].begin(); iter != sounds[i].end(); ++iter)
			iter->second->release();
		sounds[i].clear();
	}
	
	// Release system
	system->release();
}

void AudioManager::LoadSFX(const std::string& path) {
	Load(CATEGORY_SFX, path);
}

void AudioManager::LoadSong(const std::string& path) {
	Load(CATEGORY_SONG, path);
}

void AudioManager::Load(Category type, const std::string& path) {
	if (sounds[type].find(path) != sounds[type].end()) return;
	FMOD::Sound* sound;
	system->createSound(path.c_str(), modes[type], 0, &sound);
	sounds[type].insert(std::make_pair(path, sound));
}

void AudioManager::PlaySFX(const std::string& path, float minVolume, float maxVolume, float minPitch, float maxPitch) {
	// Try to find sound effect and return if not found
	SoundMap::iterator sound = sounds[CATEGORY_SFX].find(path);
	if (sound == sounds[CATEGORY_SFX].end()) return;
	
	// Calculate random volume and pitch in selected range
	float volume = RandomBetween(minVolume, maxVolume);
	float pitch = RandomBetween(minPitch, maxPitch);
	
	// Play the sound effect with these initial values
	FMOD::Channel* channel;
	system->playSound(FMOD_CHANNEL_FREE, sound->second,	true, &channel);
	channel->setChannelGroup(groups[CATEGORY_SFX]);
	channel->setVolume(volume);
	float frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	channel->setPaused(false);
}

void AudioManager::StopSFXs() {
	groups[CATEGORY_SFX]->stop();
}

void AudioManager::PlaySong(const std::string& path) {
	// Ignore if this song is already playing
	if(currentSongPath == path) return;

	// If a song is playing stop them and set this as the next song
	if(currentSong != 0) {
		StopSongs();
		nextSongPath = path;
		return;
	}

	// Search for a matching sound in the map
	SoundMap::iterator sound = sounds[CATEGORY_SONG].find(path);
	if (sound == sounds[CATEGORY_SONG].end()) return;
	
	// Start playing song with volume set to 0 and fade in
	currentSongPath = path;
	system->playSound(FMOD_CHANNEL_FREE, sound->second, true, &currentSong);
	currentSong->setChannelGroup(groups[CATEGORY_SONG]);
	currentSong->setVolume(0.0f);
	currentSong->setPaused(false);
	fade = FADE_IN;
}

void AudioManager::StopSongs() {
	if(currentSong != 0) 
		fade = FADE_OUT;
	nextSongPath.clear();
}

void AudioManager::Update(float elapsed) {
	const float fadeTime = 1.0f; // in seconds
	
	if(currentSong != 0 && fade == FADE_IN) {
		float volume;
		currentSong->getVolume(&volume);
		float nextVolume = volume + elapsed / fadeTime;
		if(nextVolume >= 1.0f) {
			currentSong->setVolume(1.0f);
			fade = FADE_NONE;
		} else {
			currentSong->setVolume(nextVolume);
		}
	} else if(currentSong != 0 && fade == FADE_OUT) {
		float volume;
		currentSong->getVolume(&volume);
		float nextVolume = volume - elapsed / fadeTime;
		if(nextVolume <= 0.0f) {
			currentSong->stop();
			currentSong = 0;
			currentSongPath.clear();
			fade = FADE_NONE;
		} else {
			currentSong->setVolume(nextVolume);
		}
	} else if(currentSong == 0 && !nextSongPath.empty()) {
		PlaySong(nextSongPath);
		nextSongPath.clear();
	}
	system->update();
}

void AudioManager::SetMasterVolume(float volume) {
	master->setVolume(volume);
}
void AudioManager::SetSFXsVolume(float volume) {
	groups[CATEGORY_SFX]->setVolume(volume);
}
void AudioManager::SetSongsVolume(float volume) {
	groups[CATEGORY_SONG]->setVolume(volume);
}