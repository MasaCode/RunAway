#include "App.h"

#include <MasaEngine/ScreenList.h>

App::App(std::string windowName, int screenWidth, int screenHeight)
{
	_windowName = windowName;
	_width = screenWidth;
	_height = screenHeight;
}


App::~App()
{
	//Empty
}

void App::onInit(){
	//Empty
}

void App::addScreens(){
	_mainMenuScreen = std::make_unique<MainMenuScreen>(&_window);
	_levelOne = std::make_unique<LevelOneScreen>(&_window);
	_levelTwo = std::make_unique<LevelTwoScreen>(&_window);

	_screenList->addScreen(_mainMenuScreen.get());
	_screenList->addScreen(_levelOne.get());
	_screenList->addScreen(_levelTwo.get());

	_screenList->setScreen(_mainMenuScreen->getScreenIndex());
}

void App::onExit(){
	//EmptyS
}

