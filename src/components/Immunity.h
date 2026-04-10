// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

struct Immunity : public ecs::Component {

	Immunity() : _isImmune(false) {
	}
	virtual ~Immunity() {

	}
	
	bool _isImmune;
};

