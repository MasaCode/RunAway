#pragma once
#include "Item.h"


class Sword : public Item
{
public:
	Sword();
	~Sword();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;


};

