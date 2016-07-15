#pragma once


#define SCREEN_INDEX_NO_SCREEN -1


namespace MasaEngine{

	class IMainGame;

	enum class ScreenState{
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};


	class IGameScreen
	{
		friend class ScreenList; // Make sure this is working correctly.

	public:
		IGameScreen()
		{
			//Empty
		}

		virtual ~IGameScreen()
		{
			//Empty
		}

		//Return the index of the next or previous screen when changing screen.
		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		//Called at beginning and end of application
		virtual void build() = 0;
		virtual void destroy() = 0;

		//Called when a screen enters and exits focus.
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//Called in the main game loop.
		virtual void update() = 0;
		virtual void draw() = 0;

		int getScreenIndex()const{
			return _screenIndex;
		}

		void setRunning(){
			_currentState = ScreenState::RUNNING;
		}

		void setState(ScreenState state){
			_currentState = state;
		}

		ScreenState getState() const {
			return _currentState;
		}

		void setParentGame(IMainGame* game){
			_game = game;
		}

	protected:
		int _screenIndex = -1;

		ScreenState _currentState = ScreenState::NONE;
		IMainGame* _game = nullptr;
	};

}