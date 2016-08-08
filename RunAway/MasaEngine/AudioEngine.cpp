#include "AudioEngine.h"
#include "MasaEngineError.h"


namespace MasaEngine{

///////////////////////////////////////////////////// SoundEffect class /////////////////////////////////////////////////////
	
	void SoundEffect::play(int loops, int volume){
		if (Mix_PlayChannel(-1, _chunk, loops) == -1){
			if (Mix_PlayChannel(0,_chunk,loops ) == -1){
				fatalError("Mix_PlayChannel error : " + std::string(Mix_GetError()));
			}
		}
		Mix_VolumeChunk(_chunk, volume);
	}


//////////////////////////////////////////////////////// Music class ////////////////////////////////////////////////////////

	void Music::play(int loops){
		if (Mix_PlayMusic(_music, loops) == -1){
			fatalError("Mix_PlayMusic error : " + std::string(Mix_GetError()));
		}
	}

	void Music::pause(){
		Mix_PauseMusic();
	}

	void Music::stop(){
		Mix_HaltMusic();
	}

	void Music::resume(){
		Mix_ResumeMusic();
	}

	void Music::setVolume(int volume) {
		Mix_VolumeMusic(volume);
	}

///////////////////////////////////////////////////// AudioEngine class /////////////////////////////////////////////////////

	AudioEngine::AudioEngine()
	{
		//Empty
	}


	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init(){

		if (_isInitialized){
			fatalError("Tried to initialize Audio Engine twice!");
		}

		//Make sure call this function after call SDL_Init() function.
		//Parameter can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD) == -1){
			fatalError("MIX_INIT error : " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1){
			fatalError("MIX_OpenAudio error : " + std::string(Mix_GetError()));
		}

		_isInitialized = true;

	}

	void AudioEngine::destroy(){
		if (_isInitialized){
			_isInitialized = false;
			
			for (auto& itr : _effectMap){
				Mix_FreeChunk(itr.second);
			}

			for (auto& itr : _musicMap){
				Mix_FreeMusic(itr.second);
			}

			_effectMap.clear();
			_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath){
		SoundEffect effect;

		//Try to find the audio in the cache
		auto itr = _effectMap.find(filePath);
		
		if (itr == _effectMap.end()){
			//Failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr){
				fatalError("MIX_LoadWAV error : " + std::string(Mix_GetError()));
			}
			
			effect._chunk = chunk;
			//Inserting the new pair into map
			_effectMap[filePath] = chunk;
		}
		else{
			//Its already cached
			effect._chunk = itr->second;
		}
		
		return effect;
	}
	
	Music AudioEngine::loadMusic(const std::string& filePath){
		Music audio;

		//Try to find the music in the cache
		auto itr = _musicMap.find(filePath);

		if (itr == _musicMap.end()){
			//Failed to find it, must load
			Mix_Music* music = Mix_LoadMUS(filePath.c_str());
			if (music == nullptr){
				fatalError("Mix_LoadMUS error : " + std::string(Mix_GetError()));
			}
			audio._music = music;
			_musicMap[filePath] = music;
		}
		else{
			//Its already cached
			audio._music = itr->second;
		}

		return audio;
	}


}