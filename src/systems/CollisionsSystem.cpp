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

	// food
	//for (auto it = _foods.begin(); it != _foods.end(); ) {

	//	auto tr = _mngr->getComponent<Transform>(it->e);

	//	bool coll =
	//		tr->_pos.getX() < pTR->_pos.getX() + pTR->_width &&
	//		tr->_pos.getX() + tr->_width > pTR->_pos.getX() &&
	//		tr->_pos.getY() < pTR->_pos.getY() + pTR->_height &&
	//		tr->_pos.getY() + tr->_height > pTR->_pos.getY();

	//	if (coll) {

	//		Message m;
	//		m.id = _m_PACMAN_FOOD_COLLISION;
	//		m.pacman_food_collision.isMagic = it->isMagic;
	//		m.pacman_food_collision.isActive = it->isActive;

	//		_mngr->send(m);

	//		it->e->setAlive(false);
	//		it = _foods.erase(it);
	//	}
	//	else {
	//		++it;
	//	}
	//}

