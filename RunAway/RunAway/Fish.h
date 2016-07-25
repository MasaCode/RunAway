#pragma once

#include "Item.h"

class GoodFish : public Item
{
public:
	GoodFish();
	~GoodFish();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;

};

class BadFish : public Item
{
public:
	BadFish();
	~BadFish();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;
};