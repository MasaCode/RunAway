#pragma once

const float PLAYER_SPEED = 5.0f;
const float ZOMBIE_SPEED = 4.5f;

const int TILE_WIDTH = 64;

const int ITEM_SIZE = 25;

//const int PLAYER_DIMS = 50;
const int PLAYER_DIM_X = 30;
const int PLAYER_DIM_Y = 40;

const int MAX_MOVEMENT = 40;

const int MONSTER_KIND = 3;
const int ITEM_KIND = 4;


enum class MonsterState{
	ATTACKING,
	ATTAKCED,
	NOMAL,
	WAITING
};
enum class MovingState{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

enum class PlayerState{
	ATTACKING,
	NOMAL
};

enum class ItemType {
	HP,
	Attack,
	Speed
};