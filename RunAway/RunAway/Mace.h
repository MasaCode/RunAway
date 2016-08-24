#pragma once
#include "Item.h"
class Mace : public Item{
public:
	Mace();
	~Mace();

	virtual void init(const glm::vec2& position) override;
	virtual void destroy() override;

};

