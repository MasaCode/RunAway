#pragma once
#include "Item.h"

class BigPotion : public Item{
public:
	BigPotion();
	~BigPotion();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;

};

class SmallPotion : public Item {
public:
	SmallPotion();
	~SmallPotion();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;
};