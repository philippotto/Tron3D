#include "AudioManager.h"
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "../globals.h"
#include "../constants.h"


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

AudioManager::AudioManager() : currentSong(0), engineChannel(0), fade(FADE_NONE) {
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
	modes[CATEGORY_SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_SOFTWARE;
	modes[CATEGORY_ENGINE] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL | FMOD_SOFTWARE;

	// Seed random number generator for SFXs
	srand((unsigned int) time(0));
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

void AudioManager::LoadEngineSound()
{
	Load(CATEGORY_ENGINE, "data/sound/engine-loop-1-normalized.wav");
}

void AudioManager::PlayEngineSound()
{
	// Search for a matching sound in the map
	SoundMap::iterator sound = sounds[CATEGORY_ENGINE].find("data/sound/engine-loop-1-normalized.wav");
	if (sound == sounds[CATEGORY_ENGINE].end()) return;
	
	system->playSound(FMOD_CHANNEL_FREE, sound->second, true, &engineChannel);
	engineChannel->setChannelGroup(groups[CATEGORY_ENGINE]);
	engineChannel->setVolume(0.1f);
	engineChannel->setPaused(false);
}

void AudioManager::Load(Category type, const std::string& path) {
	if (sounds[type].find(path) != sounds[type].end()) return;
	FMOD::Sound* sound;
	system->createSound(path.c_str(), modes[type], 0, &sound);
	sounds[type].insert(std::make_pair(path, sound));
}

void AudioManager::PlaySFX(const std::string& path, float minVolume, float maxVolume, float minPitch, float maxPitch)
{
	// Try to find sound effect and return if not found
	SoundMap::iterator sound = sounds[CATEGORY_SFX].find(path);
	if (sound == sounds[CATEGORY_SFX].end()) return;

	// Calculate random volume and pitch in selected range
	float volume = RandomBetween(minVolume, maxVolume);
	float pitch = RandomBetween(minPitch, maxPitch);

	// Play the sound effect with these initial values
	system->playSound(FMOD_CHANNEL_FREE, sound->second,	true, &m_channel);
	m_channel->setChannelGroup(groups[CATEGORY_SFX]);
	m_channel->setVolume(volume);
	float frequency;
	m_channel->getFrequency(&frequency);
	m_channel->setFrequency(ChangeSemitone(frequency, pitch));
	m_channel->setPaused(false);
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

void AudioManager::setMotorSpeed(float speed) {
	float speedFactor = (speed - BIKE_VELOCITY_MIN) / BIKE_VELOCITY_MAX;

	float currentFrequency;
	engineChannel->getFrequency(&currentFrequency);

	float desiredFrequency = ENGINE_FREQUENCY_LOW + speedFactor * ENGINE_FREQUENCY_HIGH;

	engineChannel->setFrequency(currentFrequency + (desiredFrequency - currentFrequency) / 100);
}

float AudioManager::getTimeSinceLastBeat()
{
	return m_timeSinceLastBeat;
}

void AudioManager::detectBeat(float tickCount)
{
	// getSpectrum() performs the frequency analysis, see explanation below
	int sampleSize = 64;

	float *specLeft, *specRight;

	specLeft = new float[sampleSize];
	specRight = new float[sampleSize];

	// Get spectrum for left and right stereo channels
	currentSong->getSpectrum(specLeft, sampleSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
	currentSong->getSpectrum(specRight, sampleSize, 1, FMOD_DSP_FFT_WINDOW_RECT);
	

	// get average volume distribution
	float *spec;

	spec = new float[sampleSize];

	for (int i = 0; i < sampleSize; i++)
		spec[i] = (specLeft[i] + specRight[i]) / 2;

	// Find max volume
	//auto maxIterator = std::max_element(&spec[0], &spec[sampleSize]);
	//float maxVol = *maxIterator;

	// Normalize
	// doesn't give good values? maybe sampleSize should be increased?
	// beatThresholdVolume could need tuning if soundtrack will be changed.
	// if (maxVol != 0)
	//	std::transform(&spec[0], &spec[sampleSize], &spec[0], [maxVol](float dB) -> float { return dB / maxVol; });
	
	// configuration
	float beatThresholdVolume = 0.6f;    // The threshold over which to recognize a beat
	int beatThresholdBar = 0;            // The bar in the volume distribution to examine
	float beatPostIgnore = 0.250f;		 // Number of ms to ignore track for after a beat is recognized

	static float beatLastTick = 0;                // Time when last beat occurred

	// detect beat
	bool beatDetected = false;

	// Test for threshold volume being exceeded (if not currently ignoring track)
	if (spec[beatThresholdBar] >= beatThresholdVolume && beatLastTick == 0)
	{
		beatLastTick = tickCount;
		beatDetected = true;
	}

	// If the ignore time has expired, allow testing for a beat again
	if (tickCount - beatLastTick >= beatPostIgnore)
		beatLastTick = 0;

	
	if (beatDetected)
		m_timeSinceLastBeat = 0.f;
	else
		m_timeSinceLastBeat += tickCount;

	delete[] spec;
	delete[] specLeft;
	delete[] specRight;
}

void AudioManager::Update(float elapsed)
{
	detectBeat(elapsed);

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
float AudioManager::GetMasterVolume(){
	float volume;
	master->getVolume(&volume);
	return volume;
}
void AudioManager::SetSFXsVolume(float volume) {
	groups[CATEGORY_SFX]->setVolume(volume);
}
void AudioManager::SetSongsVolume(float volume) {
	groups[CATEGORY_SONG]->setVolume(volume);
}