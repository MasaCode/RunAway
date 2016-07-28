#pragma once

#include <MasaEngine/IMainGame.h>

#include "MainMenuScreen.h"
#include "LevelOneScreen.h"
#include "LevelTwoScreen.h"


class App : public MasaEngine::IMainGame
{
public:
	App(std::string windowName, int screenWidth, int screenHeight);
	~App();

	//any custom initialzation.
	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;

private:
	std::unique_ptr<MainMenuScreen> _mainMenuScreen = nullptr;
	std::unique_ptr<LevelOneScreen> _levelOne = nullptr;
	std::unique_ptr<LevelTwoScreen> _levelTwo = nullptr;
};

