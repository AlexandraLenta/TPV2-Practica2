// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../utils/Collisions.h"


CollisionsSystem::CollisionsSystem() {
	// TODO Auto-generated constructor stub

}

CollisionsSystem::~CollisionsSystem() {
	// TODO Auto-generated destructor stub
}

void CollisionsSystem::initSystem() {
}

void CollisionsSystem::update() {

	// the PacMan's Transform
	//
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pTR = _mngr->getComponent<Transform>(pm);

	ghostCollision(pm, pTR);
	foodCollision(pm, pTR);
}	

void 
CollisionsSystem::ghostCollision(ecs::entity_t pm, Transform* pTR) {
	auto ghostList = _mngr->getEntities(ecs::grp::GHOSTS);

	// pacman <-> ghosts
	for (auto it = ghostList.begin(); it != ghostList.end(); it++) {
		auto tr = _mngr->getComponent<Transform>(*it);

		if (_mngr->isAlive(*it)) {
			if (Collisions::collides(pTR->_pos, pTR->_width, pTR->_height, tr->_pos, tr->_width, tr->_height)) {
				Message m;
				m.id = _m_PACMAN_GHOST_COLLISION;
				m.ghost_collision_data.e = *it;
				_mngr->send(m);
			}
		}
	}
}

void 
CollisionsSystem::foodCollision(ecs::entity_t pm, Transform* pTR) {
	auto foodList = _mngr->getEntities(ecs::grp::FRUIT);

	for (auto it = foodList.begin(); it != foodList.end(); it++) {

		auto tr = _mngr->getComponent<Transform>(*it);

		if (_mngr->isAlive(*it)) {
			if (Collisions::collides(pTR->_pos, pTR->_width, pTR->_height, tr->_pos, tr->_width, tr->_height)) {
				Message m;
				m.id = _m_PACMAN_FOOD_COLLISION;
				m.food_collision_data.e = *it;
				_mngr->send(m);
			}
		}
	}
}