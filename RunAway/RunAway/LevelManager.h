#pragma once

#include <string>
#include <vector>


#include <MasaEngine\SpriteBatch.h>

#include "Defenisions.h"

class LevelManager
{
public:
	// load the level
	LevelManager(const std::string& fileName);
	~LevelManager();

	void draw();

	//getters
	int getWidth();
	int getHeight();
	int getNumMonsters() const ;
	glm::vec2 getStartPlayerPosition() const;
	const std::vector<glm::vec2>& getMonstersStartPosition() const;
	const std::vector<std::string>& getLevelData() const;
	const char getSymbol(int x, int y);

private:
	std::vector<std::string> _levelData;
	int _numMonsters;

	MasaEngine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPosition;
	std::vector<glm::vec2> _monsterStartPositions;

	int _width = 0;
	int _height = 0;
};

