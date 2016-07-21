#pragma once

#include <MasaEngine/IGameScreen.h>
#include <MasaEngine/SpriteBatch.h>
#include <MasaEngine/SpriteFont.h>
#include <MasaEngine/GL_SL_Program.h>
#include <MasaEngine/Camera2D.h>
#include <MasaEngine/GLTexture.h>
#include <MasaEngine/Window.h>
#include <MasaEngine/GUI.h>
#include <MasaEngine/AudioEngine.h>

#include <memory>

#include "Defenisions.h"

#include "Indicies.h"
#include "LevelManager.h"

#include "Player.h"

//Monsters
#include "Wolf.h"
#include "Werewolf.h"
#include "Orga.h"

//Items
#include "Potion.h"
#include "Meat.h"



class LevelOneScreen : public MasaEngine::IGameScreen
{
public:
	LevelOneScreen(MasaEngine::Window* window);
	~LevelOneScreen();

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
	void initLevel();
	void loadMusic();
	void checkInput();


	//Event Functions
	bool onExitCliked(const CEGUI::EventArgs& e);

private:
	MasaEngine::Window* _window;
	MasaEngine::SpriteBatch _spriteBatch;
	MasaEngine::Camera2D _camera;
	MasaEngine::GUI _gui;
	MasaEngine::SpriteFont _spriteFont;
	MasaEngine::GLTexture _texture;
	MasaEngine::GL_SL_Program _textureProgram;
	MasaEngine::AudioEngine _audioEngine;

	std::unique_ptr<LevelManager> _level;

	Player* _player;
	
	std::vector<Monster*> _monsters;
	std::vector<Item*> _items;
	
	glm::vec2 _screenSize;
};

