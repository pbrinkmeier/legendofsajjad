#include "soundBank.hpp"

#include <iostream>

namespace los {
	std::map<const char*, Mix_Chunk*> SoundBank::m_sounds;
	
	void SoundBank::init() {
		if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
			std::cerr << "SRC: soundBank.cpp\tERR: Failed to initialize SDL_mixer\n";

		const int FREQUENCY = MIX_DEFAULT_FREQUENCY;
		const Uint16 FORMAT = MIX_DEFAULT_FORMAT;
		const int CHANNELS = 2;
		const int CHUNKSIZE = 4096;
		if (Mix_OpenAudio(FREQUENCY, FORMAT, CHANNELS, CHUNKSIZE) == -1)
			std::cerr << "SRC: soundBank.cpp\tERR: Failed to initialize audio API\n";

		Mix_Volume(-1, MIX_MAX_VOLUME / 6);

		m_sounds["patrick"] = loadSound("../res/sound/patrick.wav");
		m_sounds["penguin"] = loadSound("../res/sound/penguin.wav");
		m_sounds["playerdamage"] = loadSound("../res/sound/playerdamage.wav");
		m_sounds["projectile"] = loadSound("../res/sound/projectile.wav");
		m_sounds["bossdeath"] = loadSound("../res/sound/bossdeath.wav");
		m_sounds["maintheme"] = loadSound("../res/sound/maintheme.ogg");
		m_sounds["finalboss"] = loadSound("../res/sound/finalbosstheme.ogg");
	}

	void SoundBank::destroy() {
		for (auto pairs : m_sounds)
			Mix_FreeChunk(pairs.second);
		m_sounds.clear();

		Mix_CloseAudio();
		Mix_Quit();
	}

	void SoundBank::playSound(const char *soundName) {
		Mix_PlayChannel(-1, m_sounds.at(soundName), 0);
	}

	void SoundBank::stop() {
		Mix_HaltChannel(-1);
	}

	void SoundBank::loop(const char *soundName) {
		Mix_PlayChannel(-1, m_sounds.at(soundName), -1);
	}

	Mix_Chunk* SoundBank::loadSound(const char *file) {
		Mix_Chunk *sample = Mix_LoadWAV(file);
		if (!sample)
			std::cerr << "SRC: soundBank.cpp\tERR: Could not load file " << file << "\n";
		return sample;
	}
};
