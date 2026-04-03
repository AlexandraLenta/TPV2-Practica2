#include "ImmunitySystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"
ImmunitySystem::ImmunitySystem() :_active(false),_timer(0.0f) {

}
ImmunitySystem::~ImmunitySystem() {

}

void ImmunitySystem::initSystem() {

}
void ImmunitySystem::update() {

	if (_active) {
		_timer -= sdlutils().deltaTime();

		if (_timer <= 0.0f) {
			_active = false;

			Message m;
			m.id = _m_IMMUNITY_END;
			_mngr->send(m);
		}
	}
}

void ImmunitySystem::recieve(const Message& m) {

	switch (m.id) {

	case _m_PACMAN_FOOD_COLLISION:
		if (m.pacman_food_collision.isMagic &&m.pacman_food_collision.isActive &&!_active) {

			_active = true;
			_timer = 10.0f;

			Message msg;
			msg.id = _m_IMMUNITY_START;
			_mngr->send(msg);
		}
		break;
	case _m_NEW_GAME:
	case _m_GAME_OVER:
		_active = false;
		break;
	}
}
