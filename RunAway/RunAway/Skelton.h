#pragma once
#include "Monster.h"
class Skelton : public Monster{
public:
	Skelton();
	~Skelton();


	virtual void init(int level, float speed, const glm::vec2& position, int movementCount = 1) override;

	virtual void update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition) override;

	virtual glm::vec4 animation() override;

	virtual void destroy() override;

private:
	void ChangeDirection();

private:
	float _distanceX = 0;
	float _distanceY = 0;
	float _nearRange = 0;
	int _attackingCount = 0;
	int _baseSpeed = 0;
	const int _inRangeMovementCount = 4;

};

