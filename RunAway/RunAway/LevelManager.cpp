#include "LevelManager.h"

#include <fstream>
#include <iostream>

#include <MasaEngine\MasaEngineError.h>
#include <MasaEngine\ResourceManager.h>

#include <random>
#include <ctime>

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
	while (std::getline(file, tmp)){//this getline returns 0 or null(if failed)
		_levelData.push_back(tmp);
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
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/GroundGrass.png").id, 0.0f, whiteColor);
				break;
			case 'M':
				_monsterStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				_levelData[y][x] = '.'; // So we dont collide with a Z (zombie)
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/GroundGrass.png").id, 0.0f, whiteColor);
				break;
			case '.':
				_spriteBatch.draw(destRect, uvRect, MasaEngine::ResourceManager::getTexture("Assets/Floor/GroundGrass.png").id, 0.0f, whiteColor);
				break;
				default:
					std::cout << "Unexpected symbol" << tile << " at (" << x << " , " << y << ")" << std::endl;
					break;
			}

		}
	}

	_spriteBatch.end();

}


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