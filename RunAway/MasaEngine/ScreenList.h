#pragma once

#include <vector>

namespace MasaEngine{

	class IMainGame;
	class IGameScreen;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		void destroy();

		IGameScreen* getCurrentScreen();

	protected:
		IMainGame* _game = nullptr;
		std::vector<IGameScreen*> _screen;
		int _currentScreenIndex = -1;

	};

}