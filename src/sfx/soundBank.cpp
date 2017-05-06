#include "soundBank.hpp"

#include <iostream>

namespace los {
	std::map<const short, Mix_Chunk*> SoundBank::m_sounds;
	
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

		m_sounds[SFX_PATRICK] = loadSound("../res/sound/patrick.wav");
		m_sounds[SFX_PENGUIN] = loadSound("../res/sound/penguin.wav");
		m_sounds[SFX_PLAYERDMG] = loadSound("../res/sound/playerdamage.wav");
		m_sounds[SFX_PROJECTILE] = loadSound("../res/sound/projectile.wav");
		m_sounds[SFX_BOSSDEATH] = loadSound("../res/sound/bossdeath.wav");
		m_sounds[SFX_MAINTHEME] = loadSound("../res/sound/maintheme.ogg");
		m_sounds[SFX_FINALBOSS] = loadSound("../res/sound/finalbosstheme.ogg");
	}

	void SoundBank::destroy() {
		for (auto pairs : m_sounds)
			Mix_FreeChunk(pairs.second);
		m_sounds.clear();

		Mix_CloseAudio();
		Mix_Quit();
	}

	void SoundBank::playSound(const SOUND soundName) {
		auto it = m_sounds.find(soundName);
		if (it != m_sounds.end()) {
			if (Mix_PlayChannel(-1, it->second, 0) == -1) {
				std::string message = "SRC: soundBank.cpp\tERR: Failed to play sound ";
				std::cerr << message << soundName << " (" << Mix_GetError() << ")\n";
			}
		} else
			std::cerr << "SRC: soundBank.cpp\tERR: Could not find sound " << soundName << "\n";
	}

	void SoundBank::stop() {
		Mix_HaltChannel(-1);
	}

	void SoundBank::loop(const SOUND soundName) {
		auto it = m_sounds.find(soundName);
		if (it != m_sounds.end()) {
			if (Mix_PlayChannel(-1, it->second, -1) == -1)
				std::cerr << "SRC: soundBank.cpp\tERR: Failed to loop sound " << soundName << "\n";
		} else
			std::cerr << "SRC: soundBank.cpp\ERR: Could not find sound " << soundName << "\n";
	}

	Mix_Chunk* SoundBank::loadSound(const char *file) {
		Mix_Chunk *sample = Mix_LoadWAV(file);
		if (!sample)
			std::cerr << "SRC: soundBank.cpp\tERR: Could not load file " << file << "\n";
		return sample;
	}
};
