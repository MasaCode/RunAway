#pragma once

namespace MasaEngine{
	class FPSLimiter{
	public:
		FPSLimiter();
		~FPSLimiter();

		void init(float maxFPS);
		
		void begin();
		float end(); // will return current FPS

	private:
		void calculateFPS();
		void setMaxFPS(float maxFPS);

	private:

		unsigned int _startTicks;
		float _maxFPS;
		float _frameTime;
		float _fps;
	};

}