#pragma once

#include <SDL2/SDL_mixer.h>
#include <map>

namespace los {
	class SoundBank {
		private:
			SoundBank() {};
			~SoundBank() {};
		public:
			static void init();
			static void destroy();
		private:
			static Mix_Chunk* loadSound(const char*);
		public:
			static void loop(const char*);
			static void stop();
			static void playSound(const char*);
		private:
			static std::map<const char*, Mix_Chunk*> m_sounds;
	};
};
