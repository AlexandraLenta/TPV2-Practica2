#pragma once
#include "../ecs/System.h"
#include "PacManSystem.h"
#include <vector>

class GhostSystem : public ecs::System
{
public:
	GhostSystem() : _pacman(nullptr), _previousSpawnTime(0.0f), _lastFrameChange(0.0f) {};
	virtual ~GhostSystem() {};

	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

private:
	ecs::entity_t _pacman;

	float _previousSpawnTime;
	float _lastFrameChange;

	const float SPAWN_INTERVAL = 5000.0f;
	const float GHOST_SIZE = 55.0f;
	const float UPDATE_VEL_PROBABILITY = 0.005f;

	const float FRAME_CHANGE_INTERVAL = 250.0f;

	const float NORMAL_GHOST_SRC_ROW = 4;
	const float BLUE_GHOST_SRC_ROW = 6;
};

