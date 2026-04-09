#pragma once
#include "../ecs/System.h"
#include "PacManSystem.h"
#include <vector>

class GhostSystem : public ecs::System
{
public:
	GhostSystem() {};
	virtual ~GhostSystem() {};

	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

private:
	ecs::entity_t _pacman;

	std::vector<ecs::entity_t> _ghosts;

	float _previousSpawnTime;
	bool _pacmanImmune = false;

	const float SPAWN_INTERVAL = 5000.0f;
	const float GHOST_SIZE = 75.0f;
	const float UPDATE_VEL_PROBABILITY = 0.005;
};

