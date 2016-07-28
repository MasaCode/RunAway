#include "LevelManager.h"

#include <fstream>
#include <iostream>

#include <MasaEngine\MasaEngineError.h>
#include <MasaEngine\ResourceManager.h>

#include <random>
#include <ctime>

//Version1

LevelManager::LevelManager(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);
	
	//erro checking
	if (file.fail())
		MasaEngine::fatalError("Failed to open " + fileName);

	//Throw away the first string in tmp
	std::string tmp;
	file >> tmp >> _numMonsters;


	std::getline(file, tmp); // Throw away the first line.
	
	//Reading the data from file
	std::vector<std::string> tempLevelData;
	while (std::getline(file, tmp)){//this getline returns 0 or null(if failed)
		tempLevelData.push_back(tmp);
	}

	_levelData.resize(tempLevelData.size());
	for (size_t i = 0; i < tempLevelData.size(); i++) {
		_levelData[i] = tempLevelData[tempLevelData.size() - 1 - i];
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	MasaEngine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	char tile;

	for (size_t y = 0; y < _levelData.size(); y++){
		for (size_t x = 0; x < _levelData[y].size(); x++){
			//Grab the tile
			tile = _levelData[y][x];
			//GEet destrect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile
			switch (tile){
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				_startPlayerPosition.x = (float)(x * TILE_WIDTH);
				_startPlayerPosition.y = (float)(y * TILE_WIDTH);
				_levelData[y][x] = '.'; // So we dont collide with a @ (player)
				//_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/GroundGrass.png").id, 0.0f, whiteColor);
				break;
			case 'M':
				_monsterStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				_levelData[y][x] = '.'; // So we dont collide with a Z (zombie)
				//_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/GroundGrass.png").id, 0.0f, whiteColor);
				break;
			case '#': // This is the to go to next level.
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/nextStage.png").id, 0.0f, whiteColor);
				break;
			case '.':
				//_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/GroundGrass.png").id, 0.0f, whiteColor);
				break;
				default:
					std::cout << "Unexpected symbol" << tile << " at (" << x << " , " << y << ")" << std::endl;
					break;
			}

		}
	}

	_spriteBatch.end();

}

//Version2

//LevelManager::LevelManager(const std::string& fileName)
//{
//
//	MasaEngine::GLTexture texture = MasaEngine::ResourceManager::getTexture("Assets/Floor/FieldTiles.png");
//	_texture.init(texture, glm::ivec2(6, 6));
//
//	std::ifstream file;
//	file.open(fileName);
//
//	//erro checking
//	if (file.fail())
//		MasaEngine::fatalError("Failed to open " + fileName);
//
//	//Throw away the first string in tmp
//	std::string tmp;
//	file >> tmp >> _numMonsters;
//
//
//	std::getline(file, tmp); // Throw away the first line.
//
//	//Reading the data from file
//	std::vector<std::string> levelData;
//	while (std::getline(file, tmp)) {//this getline returns 0 or null(if failed)
//		levelData.push_back(tmp);
//	}
//
//	_levelData.resize(levelData.size());
//	for (size_t i = 0; i < levelData.size(); i++) {
//		_levelData[i] = levelData[levelData.size() - 1 - i];
//	}
//
//
//	_spriteBatch.init();
//	_spriteBatch.begin();
//
//	glm::vec4 uvRect;
//
//	MasaEngine::Color whiteColor = MasaEngine::Color(255,255,255,255);
//
//
//	char tile;
//
//	for (size_t y = 0; y < _levelData.size(); y++) {
//		for (size_t x = 0; x < _levelData[y].size(); x++) {
//			//Grab the tile
//			tile = _levelData[y][x];
//			//GEet destrect
//			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
//
//			//Process the tile
//			switch (tile) {
//				case '@':
//					_startPlayerPosition.x = (float)(x * TILE_WIDTH);
//					_startPlayerPosition.y = (float)(y * TILE_WIDTH);
//					_levelData[y][x] = '.'; // So we dont collide with a @ (player)
//					uvRect = _texture.getUVs(0);
//					break;
//				case '#':
//					uvRect = _texture.getUVs(2);
//					break;
//				case '.':
//					uvRect = _texture.getUVs(0);
//					break;
//				case 'A':
//					uvRect = _texture.getUVs(1);
//					break;
//				case 'C':
//					uvRect = _texture.getUVs(3);
//					break;
//				case 'D':
//					uvRect = _texture.getUVs(4);
//					break;
//				case 'E':
//					uvRect = _texture.getUVs(5);
//					_levelData[y][x] = '.';
//					break;
//				case 'F':
//					uvRect = _texture.getUVs(6);
//					break;
//				case 'G':
//					uvRect = _texture.getUVs(7);
//					break;
//				case 'H':
//					uvRect = _texture.getUVs(8);
//					break;
//				case 'I':
//					uvRect = _texture.getUVs(9);
//					break;
//				case 'J':
//					uvRect = _texture.getUVs(10);
//					break;
//				case 'K':
//					uvRect = _texture.getUVs(11);
//					break;
//				case 'L':
//					uvRect = _texture.getUVs(12);
//					break;
//				case 'M':
//					uvRect = _texture.getUVs(13);
//					break;
//				case 'N':
//					uvRect = _texture.getUVs(14);
//					break;
//				case 'O':
//					uvRect = _texture.getUVs(15);
//					_levelData[y][x] = '.';
//					break;
//				case 'P':
//					uvRect = _texture.getUVs(16);
//					_levelData[y][x] = '.';
//					break;
//				case 'Q':
//					uvRect = _texture.getUVs(17);
//					_levelData[y][x] = '.';
//					break;
//				case 'R':
//					uvRect = _texture.getUVs(18);
//					_levelData[y][x] = '.';
//					break;
//				case 'S':
//					uvRect = _texture.getUVs(19);
//					_levelData[y][x] = '.';
//					break;
//				case 'T':
//					uvRect = _texture.getUVs(20);
//					_levelData[y][x] = '.';
//					break;
//				case 'U':
//					uvRect = _texture.getUVs(21);
//					_levelData[y][x] = '.';
//					break;
//				case 'V':
//					uvRect = _texture.getUVs(22);
//					_levelData[y][x] = '.';
//					break;
//				case 'W':
//					uvRect = _texture.getUVs(23);
//					_levelData[y][x] = '.';
//					break;
//				case 'X':
//					uvRect = _texture.getUVs(24);
//					_levelData[y][x] = '.';
//					break;
//				case 'Y':
//					uvRect = _texture.getUVs(25);
//					_levelData[y][x] = '.';
//					break;
//				case 'Z':
//					uvRect = _texture.getUVs(26);
//					_levelData[y][x] = '.';
//					break;
//				case 'a':
//					uvRect = _texture.getUVs(27);
//					_levelData[y][x] = '.';
//					break;
//				case 'b':
//					uvRect = _texture.getUVs(28);
//					_levelData[y][x] = '.';
//					break;
//				case 'c':
//					uvRect = _texture.getUVs(29);
//					_levelData[y][x] = '.';
//					break;
//				case 'd':
//					uvRect = _texture.getUVs(30);
//					_levelData[y][x] = '.';
//					break;
//				case 'e':
//					uvRect = _texture.getUVs(31);
//					_levelData[y][x] = '.';
//					break;
//				case 'f':
//					uvRect = _texture.getUVs(32);
//					_levelData[y][x] = '.';
//					break;
//				case 'g':
//					uvRect = _texture.getUVs(33);
//					_levelData[y][x] = '.';
//					break;
//				case 'h':
//					uvRect = _texture.getUVs(34);
//					_levelData[y][x] = '.';
//					break;
//				default:
//					std::cout << "Unexpected symbol" << tile << " at (" << x << " , " << y << ")" << std::endl;
//					uvRect = _texture.getUVs(35);
//					break;
//			}
//			_spriteBatch.draw(destRect, uvRect, _texture.texture.id, 0.0f, whiteColor);
//
//		}
//	}
//
//	_spriteBatch.end();
//
//}

//Version3

//LevelManager::LevelManager(const std::string& fileName)
//{
//	std::ifstream file;
//	file.open(fileName);
//
//	//erro checking
//	if (file.fail())
//		MasaEngine::fatalError("Failed to open " + fileName);
//
//	//Throw away the first string in tmp
//	std::string tmp;
//	file >> tmp >> _numMonsters;
//
//
//	std::getline(file, tmp); // Throw away the first line.
//
//	//Reading the data from file
//	std::vector<std::string> levelData;
//	while (std::getline(file, tmp)) {//this getline returns 0 or null(if failed)
//		levelData.push_back(tmp);
//	}
//
//	_levelData.resize(levelData.size());
//	for (size_t i = 0; i < levelData.size(); i++) {
//		_levelData[i] = levelData[levelData.size() - 1 - i];
//	}
//
//
//	_spriteBatch.init();
//	_spriteBatch.begin();
//
//	glm::vec4 uvRect = glm::vec4(0.0f,0.0f,1.0f,1.0f);
//
//	MasaEngine::Color whiteColor = MasaEngine::Color(255,255,255,255);
//
//
//	char tile;
//
//	for (size_t y = 0; y < _levelData.size(); y++) {
//		for (size_t x = 0; x < _levelData[y].size(); x++) {
//			//Grab the tile
//			tile = _levelData[y][x];
//			//GEet destrect
//			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
//
//			//Process the tile
//			switch (tile) {
//				case '@':
//					_startPlayerPosition.x = (float)(x * TILE_WIDTH);
//					_startPlayerPosition.y = (float)(y * TILE_WIDTH);
//					_levelData[y][x] = '.'; // So we dont collide with a @ (player)
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/groundGrass.png").id, 0.0f, whiteColor);
//					break;
//				case '#':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/red_bricks.png").id, 0.0f, whiteColor);
//					break;
//				case '.':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/groundGrass.png").id, 0.0f, whiteColor);
//					break;
//				case 'A':	
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/glass.png").id, 0.0f, whiteColor);
//					break;
//				case 'C':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/light_bricks.png").id, 0.0f, whiteColor);
//					break;
//				case 'D':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/oldHouse.png").id, 0.0f, whiteColor);
//					break;
//				case 'E': // FLower
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Flower.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'F':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/LB.png").id, 0.0f, whiteColor);
//					break;
//				case 'G':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/MB.png").id, 0.0f, whiteColor);
//					break;
//				case 'H':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/RB.png").id, 0.0f, whiteColor);
//					break;
//				case 'I':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/LM.png").id, 0.0f, whiteColor);
//					break;
//				case 'J':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/MM.png").id, 0.0f, whiteColor);
//					break;
//				case 'K':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/RM.png").id, 0.0f, whiteColor);
//					break;
//				case 'L':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/LT.png").id, 0.0f, whiteColor);
//					break;
//				case 'M':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/MT.png").id, 0.0f, whiteColor);
//					break;
//				case 'N':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Lake/RT.png").id, 0.0f, whiteColor);
//					break;
//				case 'O':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/LB.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'P':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/MB.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'Q':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/RB.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'R':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/LM.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'S':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/MM.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'T':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/RM.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'U':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/LT.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'V':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/MT.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'W':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Ground/RT.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'X':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road5.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'Y':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road6.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'Z':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road2.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'a':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road4.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'b':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road1.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'c':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road3.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'd':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road8.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'e':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road7.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'f':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road9.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'g':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road10.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				case 'h':
//					_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/Road/Road11.png").id, 0.0f, whiteColor);
//					_levelData[y][x] = '.';
//					break;
//				default:
//					std::cout << "Unexpected symbol" << tile << " at (" << x << " , " << y << ")" << std::endl;
//					break;
//			}
//			
//
//		}
//	}
//
//	_spriteBatch.end();
//
//}

LevelManager::~LevelManager()
{
}


void LevelManager::draw(){
	_spriteBatch.renderBatch();
}


int LevelManager::getWidth(){
	return (int)_levelData[0].length(); //because this is two dimentional array and this is size of row.
}

int LevelManager::getHeight(){
	return _levelData.size(); //because this is two dimentional array and this is the size of column.
}

int LevelManager::getNumMonsters() const{
	return _numMonsters;
}

glm::vec2 LevelManager::getStartPlayerPosition() const {
	return _startPlayerPosition;
}

const std::vector<glm::vec2>& LevelManager::getMonstersStartPosition() const{
	return _monsterStartPositions;
}

const std::vector<std::string>& LevelManager::getLevelData() const{
	return _levelData;
}

const char LevelManager::getSymbol(int x, int y){
	return _levelData[y][x];
}


glm::vec2 LevelManager::getCameraPos(const glm::vec2& cameraPos, const glm::vec2& screenSize, const float scale) {
		glm::vec2 playerPos = cameraPos;
		glm::vec2 returnPos = cameraPos;

		glm::vec2 halfCameraDims = glm::vec2(screenSize.x/ (2.0f * scale), screenSize.y / (2.0f * scale));

		glm::vec2 LeftDownDistance = playerPos;
		glm::vec2 RightUpDistance = glm::vec2(getWidth()*(float)TILE_WIDTH, getHeight()*(float)TILE_WIDTH) - playerPos;

		if ((LeftDownDistance.x - halfCameraDims.x) < 0.0f) {
			//Its way too much going Left
			returnPos.x = halfCameraDims.x;
		}

		if ((LeftDownDistance.y - halfCameraDims.y) < 0.0f) {
			//Its way too much going Up
			returnPos.y = halfCameraDims.y;
		}

		if ((RightUpDistance.x - halfCameraDims.x) < 0.0f) {
			//Its way too much going Right
			returnPos.x = getWidth()*(float)TILE_WIDTH - halfCameraDims.x;
		}

		if ((RightUpDistance.y - halfCameraDims.y) < 0.0f) {
			//Its way too much going Up
			returnPos.y = getHeight()*(float)TILE_WIDTH - halfCameraDims.y;
		}


	return returnPos;
}