#pragma once
#include "Monster.h"

class Werewolf : public Monster
{
public:
	Werewolf();
	~Werewolf();

	virtual void init(int level, float speed, const glm::vec2& position, int movementCount = 1) override;

	virtual void update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition) override;

	virtual glm::vec4 animation() override;

	virtual void destroy() override;

private:
	void ChangeDirection();

private:
	float _directionX = 0;
	float _directionY = 0;
	int _attackingCount = 0;
	const int MAX_ATTACKING_MOVEMENT = 2;
//protected:
//	int m_size = 1;
//	int m_baseHitPoint = 1;
//	int m_baseAttaciPoint = 1;
//	int m_hitPoint = 0;
//	int m_attackPoint = 0;
//	int m_currentHitPoint = 0;
//	int m_level = 1;
//	int m_movementCount = 0;
//	int m_currentMovementIndex = 0;
//	int m_experiencePoint = 0;
//	float m_animTime = 0;
//	float m_speed = 0;
//	bool m_isCollided = false;
//	glm::vec2 m_position = glm::vec2(0.0f);
//	MonsterState m_state = MonsterState::NOMAL;
//	MovingState m_direction = MovingState::UP;
//	MasaEngine::Color m_color = MasaEngine::Color(255, 255, 255, 255);
//	MasaEngine::TileSheet m_texture;

};

