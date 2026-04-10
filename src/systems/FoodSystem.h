#pragma once

#include "../ecs/System.h"
#include <vector>

class FoodSystem : public ecs::System
{
public:
	FoodSystem();
	virtual ~FoodSystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

private:
	void updateMagicState();

	std::vector<ecs::entity_t> _foods;

	ecs::entity_t _pacman;

	const float FOOD_SIZE = 25.0f;
	const float GRID_SPACING = 75;
	const float MARGIN = 35.0f;

	const int FRUIT_ROW = 1;
	const int FRUIT_NORMAL_COL = 4;
	const int FRUIT_MAGIC_COL = 7;
};