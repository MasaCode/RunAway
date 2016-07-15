#include "ScreenList.h"

#include "IGameScreen.h"
#include "IMainGame.h"

namespace MasaEngine{
	
	ScreenList::ScreenList(IMainGame* game) : _game(game)
	{
		//Empty
	}
	ScreenList::~ScreenList(){
		//I think I shouldn't do this.
		//Because I make it with unique_ptr and I called unique_ptr<> screenList.reset().
		//But so far it works great
		
		destroy(); 
	}

	IGameScreen* ScreenList::moveNext(){
		IGameScreen* currentScreen = getCurrentScreen();
		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN){
			_currentScreenIndex = currentScreen->getNextScreenIndex();
		}
		return getCurrentScreen();
	}

	IGameScreen* ScreenList::movePrevious(){
		IGameScreen* currentScreen = getCurrentScreen();
		if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN){
			_currentScreenIndex = currentScreen->getPreviousScreenIndex();
		}
		return getCurrentScreen();
	}

	void ScreenList::setScreen(int nextScreen){
		_currentScreenIndex = nextScreen;
	}

	void ScreenList::addScreen(IGameScreen* newScreen){
		newScreen->_screenIndex = _screen.size();
		_screen.push_back(newScreen);
		newScreen->build();
		newScreen->setParentGame(_game);
	}

	void ScreenList::destroy(){
		for (size_t i = 0; i < _screen.size(); i++){
			_screen[i]->destroy();
		}
		_screen.resize(0);
		_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;

	}
	
	IGameScreen* ScreenList::getCurrentScreen(){
		if (_currentScreenIndex == SCREEN_INDEX_NO_SCREEN)
			return nullptr;

		return _screen[_currentScreenIndex];
	}

}