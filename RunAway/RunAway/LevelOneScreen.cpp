#include "LevelOneScreen.h"

#include <SDL/SDL.h>
#include <MasaEngine/IMainGame.h>
#include <MasaEngine/ResourceManager.h>

#include <random>
#include <ctime>



LevelOneScreen::LevelOneScreen(MasaEngine::Window* window)
	: _window(window) //, _spriteFont("Fonts/shangri-la.ttf",32)
{
	//Empty
}


LevelOneScreen::~LevelOneScreen()
{
	//Empty
}

int LevelOneScreen::getNextScreenIndex() const{
	return SCREEN_INDEX_NO_SCREEN;
}

int LevelOneScreen::getPreviousScreenIndex() const{
	return SCREEN_INDEX_MAINMENU;
}

void LevelOneScreen::build(){
	//Empty
}

void LevelOneScreen::destroy(){
	_gui.destroy();
}

void LevelOneScreen::onEntry(){
	
	//Initialize SpriteFont
	_spriteFont.init("Fonts/shangri-la.ttf", 32);

	//Initialize spriteBatch
	_spriteBatch.init();

	//Initialize the shader
	initShader();

	//Initialize GUI
	initUI();

	//Initialize audio engine
	_audioEngine.init();

	loadMusic();

	//Initialize Level
	initLevel();

	//initialize camera
	_camera.init(_window->getScreenWidth(), _window->getScreenHeight());
	//For Debuging
	//_camera.setScale(0.5f);

}

void LevelOneScreen::onExit(){
	for (size_t i = 0; i < _monsters.size(); i++){
		delete _monsters[i];
	}
	_monsters.clear();
	_gui.destroy();
	_textureProgram.dispose();
	_level.release();
	_spriteFont.dispose();
}

void LevelOneScreen::update(){
	_player->update(_game->inputManager, _level->getLevelData());
	if (_player->isPlayerDead()){
		_player->Recreate(_level->getStartPlayerPosition());
	}


	//Check if monster died or not
	for (size_t i = 0; i < _monsters.size(); i++){
		if (_monsters[i]->isDead()){
			delete _monsters[i];
			_monsters[i] = _monsters.back();
			_monsters.pop_back();
			i--;
		}
	}

	//Collision with monster and plyer
	_player->collideWithMonsters(_monsters);


	//updating all of monsters movement
	for (unsigned int i = 0; i < _monsters.size(); i++){
		_monsters[i]->update(_level->getLevelData(), _player->getPosition());
	}

	//Collision update
	for (size_t i = 0; i < _monsters.size(); i++){
		_monsters[i]->collideWithMonsters(_monsters, i);
	}

	_camera.setPosition(_player->getPosition());
	_camera.update();
	checkInput();
}



void LevelOneScreen::draw(){
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	_textureProgram.use();

	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Drawing level
	_level->draw();

	_spriteBatch.begin();

	//Draw code here
	_player->draw(_spriteBatch);

	for (unsigned int i = 0; i < _monsters.size(); i++){
		if (_camera.isBoxInView(_monsters[i]->getPosition(), glm::vec2(_monsters[i]->getSize()))){
			_monsters[i]->draw(_spriteBatch);
		}
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_textureProgram.unuse();

	_gui.draw();


}


void LevelOneScreen::initShader(){
	// Compile our texture shader
	_textureProgram.compileShader("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.LinkShaders();
}

void LevelOneScreen::initUI(){
	_gui.init("GUI");
	_gui.loadScheme("TaharezLook.scheme");
	_gui.loadScheme("AlfiskoSkin.scheme");
	_gui.setFont("DejaVuSans-10");

	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.0f, 0.0f, 0.06, 0.03f), glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("Exit Game");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LevelOneScreen::onExitCliked, this));

	_gui.setMouseCursor("TaharezLook/MouseArrow");
	_gui.showMouseCursor();
	SDL_ShowCursor(0);
}

void LevelOneScreen::initLevel(){
	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randMonster(0, 2);
	std::uniform_int_distribution<int> randomMovement(0,1000);
	glm::vec4 playerArea;

	_level = std::make_unique<LevelManager>("Levels/level1.txt");
	

	// Objects Creation code goes here.

	

	std::uniform_int_distribution<int> yPos(3, _level->getHeight() - 3);
	std::uniform_int_distribution<int> xPos(3, _level->getWidth() - 3);

	int i = 0;
	int count = 2;
	while(i < _level->getNumMonsters()){
		int temp = randMonster(randomEngine);
		int x = xPos(randomEngine);
		int y = yPos(randomEngine);
		int movement = ((randomMovement(randomEngine) * count) % MAX_MOVEMENT + count/2);
			if (_level->getSymbol(x, y) == '.'){
				glm::vec2 pos = glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH);
				if (temp == 0){
					_monsters.push_back(new Werewolf);
					_monsters.back()->init(4, 2, pos, 10);
				}
				else if (temp == 1){
					_monsters.push_back(new Orga);
					_monsters.back()->init(3, 3, pos, 30);
				}
				else{
					_monsters.push_back(new Wolf);
					_monsters.back()->init(2, 2, pos, movement);
					//std::cout << "Movement : " <<  movement << std::endl;
				}

				i++;
			}
		count++;
	}

	/*_monsters.push_back(new Wolf);
	_monsters.back()->init(2, 2, glm::vec2(500.0f), 30);*/


	//Initialize player.
	_player = Player::getInstance();
	_player->init(_level->getStartPlayerPosition(), PLAYER_SPEED,_audioEngine);

}

void LevelOneScreen::loadMusic(){
	MasaEngine::Music music = _audioEngine.loadMusic("Sound/background.ogg");
	music.play(-1);
}


void LevelOneScreen::checkInput(){
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)){
		_game->onSDLEvent(evnt);
		_gui.onSDLEvent(evnt);
	}
}



bool LevelOneScreen::onExitCliked(const CEGUI::EventArgs& e){
	_currentState = MasaEngine::ScreenState::EXIT_APPLICATION;
	return true;
}
