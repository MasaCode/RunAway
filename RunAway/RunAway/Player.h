#pragma once



#include <MasaEngine/InputManager.h>
#include <MasaEngine/AudioEngine.h>

#include "Monster.h"



class Player
{
public:

	~Player();

	static Player* getInstance();

	void init(glm::vec2& position, float speed,MasaEngine::AudioEngine& audioEngine);

	glm::vec4 animation();

	void draw(MasaEngine::SpriteBatch& _spriteBatch);

	void update(MasaEngine::InputManager& inputManager, const std::vector<std::string>& levelData);

	void Recreate(const glm::vec2& pos);

	bool takeDamage(int damage);

	bool collideWithLevel(const std::vector<std::string>& levelData);

	void collideWithMonsters(std::vector<Monster*>& monsters);

	bool isAttackingFast(){ return (_animTime > 4.0f && _playerState == PlayerState::ATTACKING); }

	bool isPlayerDead(){ return _isDead; };
	
	bool isStartedDeathAnimation(){ return _startDeathAnimation; }

	void getExperiencePoint(int exp);

	/*glm::vec2 getPosition(){ return _position; }*/
	glm::vec2 getPosition(){ return _position + glm::vec2(_substructWidth,_substructHeight); }
	glm::vec2 getCutSize(){ return glm::vec2(_substructWidth, _substructHeight); }

private:
	//This is singleton design pattern.
	Player();

	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);
	void collideWithTile(glm::vec2 tilePos,bool isRight = false);
	bool collideWithMonster(Monster* monster);
	void calculateHitPoint();
	bool Attack(const glm::vec2& distance);

private:
	static Player* _instance;

	bool _isStanding = false;

	glm::vec2 _position;
	glm::vec2 _direction = glm::vec2(1.0f, 0.0);
	float _speed = 0;
	float _health = 0;
	float _animTime = 0.0f;

	MasaEngine::Color _color;
	MovingState _state;
	PlayerState _playerState = PlayerState::NOMAL;
	int _multiplyer = 1;
	const int MAX_RUN_COUNT = 30;
	int _runCount = 0;
	bool _couldRun = true;
	bool _finishAttacking = false;
	bool _startDeathAnimation = false;
	bool _isDead = false;
	int _counter = 0;

	MasaEngine::TileSheet _texture;
	MasaEngine::TileSheet _attackTexture;
	MasaEngine::GLTexture _hpBar;
	MasaEngine::GLTexture _hitPoint;
	MasaEngine::Color _hpColor;
	float _baseHitPoint = 100.0f;
	float _maxHitPoint = 100.0f;
	float _currentHitPoint = 100.0f;
	float _hpPercentage = 1.0f;
	float _experiencePoint = 0;
	float _needExperiencePoint = 0;
	float _hpSize = 0;
	int _hpGreen = 255;
	int _playerAlpha = 255;

	int _level = 1;
	int _baseAttackPoint = 5;
	int _currentAttackPoint = 5;

	//int _size = 0;
	glm::vec2 _size = glm::vec2(0.0f);

	int _additionalWidth = 0;
	int _additionalHeight = 0;

	int _substructWidth = 0;
	int _substructHeight = 0;

	int _cutSize = 0;

	int _numTiles = 0;

	MasaEngine::SoundEffect _levelUp;

};
