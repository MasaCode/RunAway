#include "Timing.h"
#include <SDL\SDL.h>


namespace MasaEngine{
	FPSLimiter::FPSLimiter(){

	}
	FPSLimiter::~FPSLimiter(){

	}

	void FPSLimiter::init(float maxFPS){
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS){
		_maxFPS = maxFPS;
	}

	void FPSLimiter::begin(){
		_startTicks = SDL_GetTicks();
	}

	float FPSLimiter::end(){
		calculateFPS();

		float frameTicks = (float)(SDL_GetTicks() - _startTicks);
		//Limit the FPS to the max FPS
		if (1000.0f / _maxFPS > frameTicks){
			SDL_Delay((Uint32)((1000.0f / _maxFPS) - frameTicks));
		}

		return _fps;
	}


	void FPSLimiter::calculateFPS(){
		static const int NUM_SAMPLES = 10;
		static float frameTime[NUM_SAMPLES];
		static int currentFrameIndex = 0;

		static float prevTicks = (float)SDL_GetTicks();

		float currentTicks;
		currentTicks = (float)SDL_GetTicks();

		//Calculating _frameTime!!!!!!!!!!!!!!!!!
		_frameTime = currentTicks - prevTicks; // frameTime

		//Inserting _frameTime into frameTime array to calculate average frameTime.
		frameTime[currentFrameIndex % NUM_SAMPLES] = _frameTime;

		prevTicks = (float)SDL_GetTicks();

		int count;

		currentFrameIndex++;

		count = currentFrameIndex < NUM_SAMPLES ? currentFrameIndex : NUM_SAMPLES;


		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++){
			frameTimeAverage += frameTime[i];
		}
		frameTimeAverage /= count;

		//This is the average of frameTime
		_fps = frameTimeAverage > 0 ? 1000.0f / frameTimeAverage : 60.0f;
	}

}