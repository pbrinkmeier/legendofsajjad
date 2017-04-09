#pragma once

#include <SDL2/SDL_mixer.h>
#include <map>

namespace los {
	class SoundBank {
		public:
			enum SOUND {
				SFX_PATRICK,
				SFX_PENGUIN,
				SFX_PLAYERDMG,
				SFX_PROJECTILE,
				SFX_BOSSDEATH,
				SFX_MAINTHEME,
				SFX_FINALBOSS
			};
		private:
			SoundBank() {};
			~SoundBank() {};
		public:
			static void init();
			static void destroy();
		private:
			static Mix_Chunk* loadSound(const char*);
		public:
			static void loop(const SOUND);
			static void stop();
			static void playSound(const SOUND);
		private:
			static std::map<const short, Mix_Chunk*> m_sounds;
	};
};
