#include "ImmunitySystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"
ImmunitySystem::ImmunitySystem() :_active(false) {

}
ImmunitySystem::~ImmunitySystem() {

}

void ImmunitySystem::initSystem() {

}
void ImmunitySystem::update() {
	auto& vT = sdlutils().virtualTimer();

	if (_active) {
		if (vT.currTime() - _lastTime) {
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
			if (m.food_collision_data.f.isMagic && m.food_collision_data.f.isActive && !_active) {
				_active = true;
				_lastTime = sdlutils().virtualTimer().currTime();

				Message msg;
				msg.id = _m_IMMUNITY_START;
				_mngr->send(msg);
			}
	}
}
