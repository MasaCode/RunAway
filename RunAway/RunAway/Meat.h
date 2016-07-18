#pragma once
#include "Item.h"

class GoodMeat : public Item
{
public:
	GoodMeat();
	~GoodMeat();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;
};

class BadMeat : public Item
{
public:
	BadMeat();
	~BadMeat();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;
};