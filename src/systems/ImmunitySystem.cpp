#include "ImmunitySystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"
#include "../components/FoodInfo.h"

ImmunitySystem::ImmunitySystem() :_active(false) {

}
ImmunitySystem::~ImmunitySystem() {

}

void ImmunitySystem::initSystem() {
	_lastTime = sdlutils().virtualTimer().currTime();
}
void ImmunitySystem::update() {
	auto& vT = sdlutils().virtualTimer();

	if (_active) {
		if (vT.currTime() - _lastTime >= IMMUNITY_TIME) {
			_lastTime = vT.currTime();
			_active = false;

			Message m;
			m.id = _m_IMMUNITY_END;
			_mngr->send(m);
		}
	}
}

void ImmunitySystem::recieve(const Message& m) {
	switch (m.id) {
		case _m_NEW_GAME:
		case _m_ROUND_START:
			_lastTime = sdlutils().virtualTimer().currTime();
			break;

		case _m_PACMAN_FOOD_COLLISION:
			auto* fInfo = _mngr->getComponent<FoodInfo>(m.food_collision_data.e);
			if (fInfo->_isMagic && fInfo->_isActive && !_active) { // if the fruit is magic, active, and we weren't already in immunity state
				_active = true;
				_lastTime = sdlutils().virtualTimer().currTime();

				Message msg;
				msg.id = _m_IMMUNITY_START;
				_mngr->send(msg);
			}
			break;
	}
}