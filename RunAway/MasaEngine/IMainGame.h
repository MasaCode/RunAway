#pragma once

#include <memory>


#include "MasaEngine.h"
#include "Window.h"
#include "InputManager.h"

#include <string>


namespace MasaEngine{

	class IGameScreen;
	class ScreenList;

	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void run();
		void exitGame();
		void onSDLEvent(SDL_Event& evnt);

		//any custom initialzation.
		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		const float getFps() const;

		bool isDestroying();

	protected:
		virtual void update();
		virtual void draw();

		bool init();
		bool initSystems();

	public:
		InputManager inputManager;

	protected:
		std::unique_ptr<ScreenList> _screenList = nullptr;
		IGameScreen* _currentScreen = nullptr;
		bool _isRunning = false;
		float _fps = 0.0f;
		Window _window;

		int _width = 1500;
		int _height = 800;
		std::string _windowName = "Default";

		bool _isDestroying = false;

	};

}