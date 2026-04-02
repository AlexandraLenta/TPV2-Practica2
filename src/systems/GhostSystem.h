#pragma once
#include "../ecs/System.h"
#include "PacManSystem.h"
#include <vector>

class GhostSystem : public ecs::System
{
public:
	GhostSystem();
	virtual ~GhostSystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

private:
	ecs::entity_t _pacman;

	std::vector<ecs::entity_t> _ghosts;

	float _spawnTimer;
	bool _pacmanImmune;
};

