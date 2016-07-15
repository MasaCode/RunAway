#pragma once
#include "Monster.h"
class Orga : public Monster
{
public:
	Orga();
	~Orga();

	virtual glm::vec4 animation() override;
	virtual void init(int level, float speed, const glm::vec2& position, int movementCount = 1) override;
	virtual void destroy() override;
	virtual void update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition) override;

private:
	void changeDirection();

private:
	float _multiplyer = 1.0f;
	float _directionX = 0.0f;
	float _directionY = 0.0f;
	bool _isChangingState = false;

//protected:
//
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
//	int m_additionalWidthForAttacking = 0;
//	int m_additionalHeightForAttacking = 0;
//	int m_followingArea = 0;
//	int m_attackingArea = 0;
//	float m_animTime = 0;
//	float m_speed = 0;
//	bool m_isCollided = false;
//	glm::vec2 m_position = glm::vec2(0.0f);
//	MonsterState m_state = MonsterState::NOMAL;
//	MovingState m_direction = MovingState::UP;
//	MasaEngine::Color m_color = MasaEngine::Color(255, 255, 255, 255);
//	MasaEngine::TileSheet m_texture;
//	MasaEngine::TileSheet m_attackMotion;

};

