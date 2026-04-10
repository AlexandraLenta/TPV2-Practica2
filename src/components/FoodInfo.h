// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

struct FoodInfo : public ecs::Component {
	FoodInfo() {

	}

	FoodInfo(bool m, float changeT, float aF, float aT) 
		: _isMagic(m), _isActive(false), _lastChangeTime(changeT), _activeFrecuency(aF), _activeTime(aT) {
	}

	virtual ~FoodInfo() {

	}

	bool _isMagic;
	bool _isActive; // miracle state

	float _lastChangeTime;
	float _activeFrecuency; // time between activations
	float _activeTime; // time of activation (changes for each activation)
};