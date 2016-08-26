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
	return SCREEN_INDEX_LEVELTWO;
}

int LevelOneScreen::getPreviousScreenIndex() const{
	return SCREEN_INDEX_MAINMENU;
}

void LevelOneScreen::build(){
	//Empty
}

void LevelOneScreen::destroy(){

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

	_screenSize = glm::vec2(_window->getScreenWidth(), _window->getScreenHeight());

	//initialize camera
	_camera.init(_window->getScreenWidth(), _window->getScreenHeight());
	//For Debuging
	_camera.setScale(1.3);

	//Initialize debuger.
	_debuger.init();

}

void LevelOneScreen::onExit(){
	for (size_t i = 0; i < _monsters.size(); i++){
		delete _monsters[i];
	}
	_monsters.clear();
	for (size_t i = 0; i < _items.size(); i++) {
		delete _items[i];
	}
	_items.clear();

	_gui.destroy();
	_textureProgram.dispose();
	_level.release();
	_spriteFont.dispose();
	_spriteBatch.dispose();
	
	_audioEngine.destroy();

	_debuger.dispose();

	_player->setReachedState(false);
}

void LevelOneScreen::update(){
	
	checkInput();

	if (_game->isDestroying())
		return;

	//Update the player and monsters.
	updateObject();

	_camera.setPosition(_level->getCameraPos(_player->getPosition(),_screenSize, _camera.getScale()));
	_camera.update();

	if (_needToDrawGoal) {
		
		glm::vec2 centerPos = _player->calculatePositionInGrid();
		if (_level->getSymbol((int)centerPos.x, (int)centerPos.y) == '#') {
			_player->setReachedState(true);
			_currentState = MasaEngine::ScreenState::CHANGE_NEXT;
		}
	}

	if ((int)_monsters.size() == 0 && !_needToDrawGoal) {
		_needToDrawGoal = true;
		MasaEngine::SoundEffect openDoor = _audioEngine.loadSoundEffect("Sound/openDoor.ogg");
		openDoor.play();
	}

}



void LevelOneScreen::draw(){

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor((12.0/255.0), (126.0/255.0), (20.0/255.0), 1.0);

	_textureProgram.use();

	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(projectionMatrix[0][0]));

	//Drawing level
	_level->draw();

	_spriteBatch.begin();

	//Draw code here
	_player->draw(_spriteBatch);

	//Drawing monsters
	for (unsigned int i = 0; i < _monsters.size(); i++){
		if (_camera.isBoxInView(_monsters[i]->getPosition(), _monsters[i]->getSize())){
			_monsters[i]->draw(_spriteBatch);
		}
	}

	//Drawing items
	for (size_t i = 0; i < _items.size(); i++) {
		if (_camera.isBoxInView(_items[i]->getPosition(), _items[i]->getSize())) {
			_items[i]->draw(_spriteBatch);
		}
	}

	//Draw the black hole (goal) to next stage.
	if (_needToDrawGoal) {
		_spriteBatch.draw(glm::vec4(_level->getGoalPosition().x, _level->getGoalPosition().y, TILE_WIDTH, TILE_WIDTH), glm::uvec4(0.0f, 0.0f, 1.0f, 1.0f), 
						  MasaEngine::ResourceManager::getTexture("Assets/Floor/nextStage.png").id, 0.0f, MasaEngine::Color(255, 255, 255, 255));
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_textureProgram.unuse();


	if (_isDebugMode) {
		drawDebug(projectionMatrix);
	}


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
	std::uniform_int_distribution<int> randMonster(0, MONSTER_KIND-1);
	std::uniform_int_distribution<int> randomMovement(0,1000);
	std::uniform_int_distribution<int> randomItemNum(1, 10);
	std::uniform_int_distribution<int> randomItemKind(0, ITEM_KIND - 1);
	glm::vec4 playerArea;

	_level = std::make_unique<LevelManager>("Levels/Version1/level1.txt");
	

	// Objects Creation code goes here.

	

	std::uniform_int_distribution<int> yPos(3, _level->getHeight() - 3);
	std::uniform_int_distribution<int> xPos(3, _level->getWidth() - 3);

	//Monsters
	int i = 0;
	int count = 2;
	int numMonster = _level->getNumMonsters();
	_monsters.reserve(numMonster);
	while(i < numMonster){
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
				else if (temp == 2) {
					_monsters.push_back(new Skelton);
					_monsters.back()->init(2, 3, pos, 20);
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

	//Testing
	/*_monsters.push_back(new Skelton);
	_monsters.back()->init(3, 2, glm::vec2(500, 1000),20);*/


	int numItem = randomItemNum(randomEngine);
	int j = 0;
	while (j < numItem) {
		int temp = randomItemKind(randomEngine);
		int x = xPos(randomEngine);
		int y = yPos(randomEngine);
		if (_level->getSymbol(x, y) == '.') {
			glm::vec2 pos = glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH);
	
			switch (temp) {
				case 0:
					_items.push_back(new BigPotion);
					break;
				case 1:
					_items.push_back(new SmallPotion);
					break;
				case 2:
					_items.push_back(new GoodMeat);
					break;
				case 3:
					_items.push_back(new BadMeat);
					break;
				case 4:
					_items.push_back(new GoodFish);
					break;
				case 5:
					_items.push_back(new BadFish);
			}

			_items.back()->init(pos);
			j++;
		}
	}

	//Initialize player.
	_player = Player::getInstance();
	_player->init(_level->getStartPlayerPosition(), PLAYER_SPEED);


	//Testing for weapon.
	_items.push_back(new Mace);
	_items.back()->init(glm::vec2(200, 1000));


}

void LevelOneScreen::loadMusic(){
	_bgm = _audioEngine.loadMusic("Sound/background.ogg");
	_bgm.setVolume(50);
	_bgm.play(-1);
}


void LevelOneScreen::checkInput(){
	SDL_Event evnt;

	_game->inputManager.update();

	while (SDL_PollEvent(&evnt)){
		_game->onSDLEvent(evnt);
		_gui.onSDLEvent(evnt);
	}

	if (_game->inputManager.isKeyPressed(SDLK_d)) {
		_isDebugMode = !_isDebugMode;
	}

}

void LevelOneScreen::updateObject() {
	_player->update(_game->inputManager, _level->getLevelData());
	if (_player->isPlayerDead()) {
		_player->Recreate(_level->getStartPlayerPosition());
	}


	//Check if monster died or not
	for (size_t i = 0; i < _monsters.size(); i++) {
		if (_monsters[i]->isDead()) {
			delete _monsters[i];
			_monsters[i] = _monsters.back();
			_monsters.pop_back();
			i--;
		}
	}

	//Check if item is taken or not
	for (size_t i = 0; i < _items.size(); i++) {
		if (_items[i]->isDisappeared()) {
			delete _items[i];
			_items[i] = _items.back();
			_items.pop_back();
			i--;
		}
	}

	//Collision with monster and plyer
	_player->collideWithMonsters(_monsters);
	_player->collideWithItems(_items);


	//updating all of monsters movement
	for (unsigned int i = 0; i < _monsters.size(); i++) {
		_monsters[i]->update(_level->getLevelData(), _player->getPosition());
	}

	//Collision update
	for (size_t i = 0; i < _monsters.size(); i++) {
		_monsters[i]->collideWithMonsters(_monsters, i);
		_monsters[i]->collideWithItems(_items);
	}
}


void LevelOneScreen::drawDebug(glm::mat4& projectionMatrix) {
	glm::vec4 destRect;
	
	//gonna draw debug line for monsters
	for (size_t i = 0; i < _monsters.size(); i++) {
		_monsters[i]->drawDebug(_debuger);
	}
	
	//gonna draw debug line for items.
	for (size_t i = 0; i < _items.size(); i++) {
		_items[i]->drawDebug(_debuger);
	}

	//gonna draw debug line for player.
	_player->drawDebug(_debuger);


	_debuger.end();
	_debuger.render(projectionMatrix, 2.0);

}


bool LevelOneScreen::onExitCliked(const CEGUI::EventArgs& e){
	_currentState = MasaEngine::ScreenState::EXIT_APPLICATION;
	return true;
}
