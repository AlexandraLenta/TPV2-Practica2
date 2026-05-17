// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"

struct GhostInfo : public ecs::Component {
	GhostInfo() : _isWeak(false), _hitsLeft(0) {
	}

	GhostInfo(bool w, int h, Vector2D p)
		: _isWeak(w), _hitsLeft(h), _originalPos(p) {
	}

	virtual ~GhostInfo() {

	}

	bool _isWeak;
	int _hitsLeft;
	Vector2D _originalPos;
};