#pragma once

#include "../ecs/System.h"
#include <vector>

struct Transform;

class FoodSystem : public ecs:: System
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
		bool isActive; // estado milagro
	
		float lastChangeTime;
		float activeFrecuency; // tiempo entre activaciones
		float activeTime;
	};

private:
	void updateMagicState();
	void checkCollisions();
	void removeFood(ecs::entity_t e);
	void clearAll();

	std::vector<Food> _foods;

	ecs::entity_t _pacman;

	float _size;
	float _spacing;

	const float FOOD_SIZE = 50.0f;
	const float GRID_SPACING = 75.0f;
};

