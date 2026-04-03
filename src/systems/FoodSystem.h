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
		float nextChange;
	};

	std::vector<Food> _foods;

	ecs::entity_t _pacman;

	bool _pacmanImmune;
	float _immunityTimer;
};

