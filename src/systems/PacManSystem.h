// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/System.h"

struct Transform;

class PacManSystem: public ecs::System {
public:

	PacManSystem();
	virtual ~PacManSystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message& m);
private:
	ecs::entity_t _pacman;
	Transform *_pmTR;
	float _lastFrameChange;
	
	const float FRAME_CHANGE = 250.0f;

	const float PACMAN_SRC_ROW = 0;
	const float PACMAN_SRC_COL = 0;

	const int HP = 3;
};

