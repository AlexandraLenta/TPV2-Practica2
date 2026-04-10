// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class Transform;

class CollisionsSystem: public ecs::System {
public:

	CollisionsSystem();
	virtual ~CollisionsSystem();
	void initSystem() override;
	void update() override;

private:
	void ghostCollision(ecs::entity_t pm, Transform* pTR);
	void foodCollision(ecs::entity_t pm, Transform* pTR);
};

