// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

struct Health : public ecs::Component {

	Health(int hp) : _hp(hp) {
	}
	virtual ~Health() {

	}

	int _hp;
};

