#include "IMainGame.h"

#include "IGameScreen.h"
#include "ScreenList.h"
#include "Timing.h"



namespace MasaEngine{

	IMainGame::IMainGame()
	{
		_screenList = std::make_unique<ScreenList>(this);
	}


	IMainGame::~IMainGame()
	{
		//Empty
	}


	void IMainGame::run(){
	
		if (!init()) return;
		
		FPSLimiter limiter;
		limiter.init(60.0f);

		_isRunning = true;
		while (_isRunning){
			limiter.begin();

			inputManager.update();
			update();
			if (_isRunning){
				draw();

				_window.swapBuffer();
				_fps = limiter.end();
			}
		}
	
	}

	void IMainGame::exitGame(){
		_currentScreen->onExit();
		if (_screenList){
			_screenList->destroy();
			_screenList.reset();
		}
		_isRunning = false;
	}

	void IMainGame::update(){
		if (_currentScreen){
			switch (_currentScreen->getState()){
				case ScreenState::RUNNING:
					_currentScreen->update();
					break;
				case ScreenState::CHANGE_NEXT:
					_currentScreen->onExit();
					_currentScreen = _screenList->moveNext();
					if (_currentScreen){
						_currentScreen->setRunning();
						_currentScreen->onEntry();
					}
					break;
				case ScreenState::CHANGE_PREVIOUS:
					_currentScreen->onExit();
					_currentScreen = _screenList->movePrevious();
					if (_currentScreen){
						_currentScreen->setRunning();
						_currentScreen->onEntry();
					}
					break;
				case ScreenState::EXIT_APPLICATION:
					exitGame();
					break;
				default:
					break;
			}
		}
		else{
			exitGame();
		}
	}
	
	void IMainGame::draw(){
		glViewport(0, 0, _window.getScreenWidth(), _window.getScreenHeight());
		if (_currentScreen && _currentScreen->getState() == ScreenState::RUNNING){
			_currentScreen->draw();
		}
	}

	void IMainGame::onSDLEvent(SDL_Event& evnt){
		
		switch (evnt.type){
			case SDL_QUIT:
				exitGame();
				break;
			case SDL_MOUSEMOTION:
				inputManager.setMouseCorrds((float)evnt.motion.x, (float)evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				inputManager.presskey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager.presskey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager.releaseKey(evnt.button.button);
				break;
		}

	}


	bool IMainGame::init(){
		MasaEngine::init();

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		if (!initSystems()) return false;

		//Custom initialization.
		onInit();

		addScreens();

		_currentScreen = _screenList->getCurrentScreen();
		_currentScreen->onEntry();
		_currentScreen->setRunning();

		return true;
		
	}

	bool IMainGame::initSystems(){
		_window.create(_windowName, _width, _height, 0);
		return true;
	}

	const float IMainGame::getFps() const{
		return _fps;
	}

}