// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../components/Points.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"

GameCtrlSystem::GameCtrlSystem() :
		_score() {
	// TODO Auto-generated constructor stub

}

GameCtrlSystem::~GameCtrlSystem() {
	// TODO Auto-generated destructor stub
}

void GameCtrlSystem::initSystem() {
}

void GameCtrlSystem::update() {
	auto &ihldr = ih();

	if (ihldr.keyDownEvent()) {
	}
}

void GameCtrlSystem::recieve(const Message &m) {
	switch (m.id) {
	default:
		break;
	}
}
