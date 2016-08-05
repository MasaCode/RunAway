#pragma once

#include <SDL/SDL_mixer.h>

#include <string>
#include <map>

namespace MasaEngine{

	class SoundEffect{
			friend class AudioEngine;

		public:
			//Plays the effect file
			//@param loops : If loops == -1, loop forever,
			// otherwise play it loops+1 times
			void play(int loops = 0);

		private:
			Mix_Chunk* _chunk = nullptr;
	};


	class Music{
			friend class AudioEngine;
		public:
			//Plays the audio file
			//@param loops : If loops == -1, loop forever,
			// otherwise play it loops times
			void play(int loops = 1);

			void pause();
			void stop();
			void resume();
			void setVolume(int volume = 128);

		private:
			Mix_Music* _music = nullptr;
	};


	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:

		std::map<std::string, Mix_Chunk*> _effectMap;
		std::map<std::string, Mix_Music*> _musicMap;

		bool _isInitialized = false;

	};
}

