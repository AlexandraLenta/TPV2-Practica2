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

	struct Food {
		ecs::entity_t e;
		bool isMagic;
		bool isActive; // miracle state
	
		float lastChangeTime;
		float activeFrecuency; // time between activations
		float activeTime; // time of activation (changes for each activation)
	};

private:
	void updateMagicState();

	std::vector<Food> _foods;

	ecs::entity_t _pacman;

	const float FOOD_SIZE = 50.0f;
	const float GRID_SPACING = 75.0f;
};

