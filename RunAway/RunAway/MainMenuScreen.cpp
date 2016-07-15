#include "MainMenuScreen.h"

#include <SDL/SDL.h>
#include <MasaEngine/IMainGame.h>
#include <MasaEngine/ResourceManager.h>

MainMenuScreen::MainMenuScreen(MasaEngine::Window* window) : _window(window)
{
	//Empty
}


MainMenuScreen::~MainMenuScreen()
{
	//Empty
}

int MainMenuScreen::getNextScreenIndex() const{
	return SCREEN_INDEX_LEVELONE;
}

int MainMenuScreen::getPreviousScreenIndex() const{
	return SCREEN_INDEX_NO_SCREEN;
}


void MainMenuScreen::build(){

}

void MainMenuScreen::destroy(){
	_gui.destroy();
}

void MainMenuScreen::onEntry(){
	//Initialize spriteBatch
	_spriteBatch.init();

	//Load the text
	//_texture = MasaEngine::ResourceManager::getTexture("");
	
	//Initialize the shader
	initShader();

	//Initialize the camera
	_camera.init(_window->getScreenWidth(), _window->getScreenHeight());

	//Initialize the GUI
	initUI();


}

void MainMenuScreen::onExit(){

}


void MainMenuScreen::update(){
	_camera.update();
	checkInput();
}

void MainMenuScreen::draw(){
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	_textureProgram.use();

	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);


	_spriteBatch.begin();

	//Draw code here.

	_spriteBatch.end();
	_spriteBatch.renderBatch();


	_textureProgram.unuse();

	_gui.draw();

	//_window->swapBuffer(); // Swaping the window

}

void MainMenuScreen::initShader(){
	// Compile our texture shader
	_textureProgram.compileShader("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.LinkShaders();
}

void MainMenuScreen::initUI(){
	_gui.init("GUI");
	_gui.loadScheme("TaharezLook.scheme");
	_gui.loadScheme("AlfiskoSkin.scheme");
	_gui.setFont("DejaVuSans-10");

	{ //For Buttons.
		//Creating Move next button
		CEGUI::PushButton* moveNextButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.4f, 0.4f, 0.2f, 0.05f), glm::vec4(0.0f), "MoveNextButton"));
		moveNextButton->setText("Play Game");
		moveNextButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onMoveNextClicked, this));

		//Creating exit button
		CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(_gui.createWidget("AlfiskoSkin/Button", glm::vec4(0.4f, 0.6f, 0.2f, 0.05f), glm::vec4(0.0f), "ExitButton"));
		exitButton->setText("Exit Game");
		exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));
	}

	_gui.setMouseCursor("TaharezLook/MouseArrow");
	_gui.showMouseCursor();
	SDL_ShowCursor(0);
}

void MainMenuScreen::checkInput(){
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)){
		_game->onSDLEvent(evnt);
		_gui.onSDLEvent(evnt);
	}
}

bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs& e){
	_currentState = MasaEngine::ScreenState::EXIT_APPLICATION;
	return true;
}

bool MainMenuScreen::onMoveNextClicked(const CEGUI::EventArgs& e){
	_currentState = MasaEngine::ScreenState::CHANGE_NEXT;
	return true;
}