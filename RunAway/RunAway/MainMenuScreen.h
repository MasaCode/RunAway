#pragma once

#include <MasaEngine/IGameScreen.h>
#include <MasaEngine/SpriteBatch.h>
#include <MasaEngine/GL_SL_Program.h>
#include <MasaEngine/Camera2D.h>
#include <MasaEngine/GLTexture.h>
#include <MasaEngine/Window.h>
#include <MasaEngine/GUI.h>

#include "Indicies.h"


class MainMenuScreen : public MasaEngine::IGameScreen
{
public:
	MainMenuScreen(MasaEngine::Window* window);
	~MainMenuScreen();


	//Return the index of the next or previous screen when changing screen.
	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	//Called at beginning and end of application
	virtual void build() override;
	virtual void destroy() override;

	//Called when a screen enters and exits focus.
	virtual void onEntry() override;
	virtual void onExit() override;

	//Called in the main game loop.
	virtual void update() override;
	virtual void draw() override;

private:
	void initShader();
	void initUI();
	void checkInput();
	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onMoveNextClicked(const CEGUI::EventArgs& e);

private:
	MasaEngine::Window* _window;
	MasaEngine::Camera2D _camera;
	MasaEngine::GUI _gui;

	MasaEngine::GL_SL_Program _textureProgram;
	MasaEngine::GLTexture _texture;
	MasaEngine::SpriteBatch _spriteBatch;

};

