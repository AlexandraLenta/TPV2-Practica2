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

private:
	struct Food {ecs::entity_t e;bool isMagic;bool isActive; // estado milagro
		float timer;
		float nextChange; //n o m segun el estado
	};

	void updateMagicState();
	void checkCollisions();
	void removeFood(ecs::entity_t e);
	void clearAll();

	std::vector<Food> _foods;

	ecs::entity_t _pacman;

	float _size;
	float _spacing;
};

