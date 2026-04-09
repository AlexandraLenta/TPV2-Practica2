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

	// ghost
	for (auto it = ghostList.begin(); it != ghostList.end(); ) {

		auto tr = _mngr->getComponent<Transform>(*it);
		auto pmTr = _mngr->getComponent<Transform>(pm);

		bool coll = tr->_pos.getX() < pmTr->_pos.getX() + pmTr->_width && tr->_pos.getX() + tr->_width > pmTr->_pos.getX() &&
			tr->_pos.getY() < pmTr->_pos.getY() + pmTr->_height && tr->_pos.getY() + tr->_height > pmTr->_pos.getY();

		if (coll) {
			if (_pacmanImmune) {
				_mngr->setAlive(*it, false);
				it = ghostList.erase(it);
				continue;
			}
			else {
				Message m;
				m.id = _m_ROUND_OVER;
				_mngr->send(m);
			}
		}

		++it;
	}

	// food
	for (auto it = _foods.begin(); it != _foods.end(); ) {

		auto tr = _mngr->getComponent<Transform>(it->e);

		bool coll =
			tr->_pos.getX() < pmTr->_pos.getX() + pmTr->_width &&
			tr->_pos.getX() + tr->_width > pmTr->_pos.getX() &&
			tr->_pos.getY() < pmTr->_pos.getY() + pmTr->_height &&
			tr->_pos.getY() + tr->_height > pmTr->_pos.getY();

		if (coll) {

			Message m;
			m.id = _m_PACMAN_FOOD_COLLISION;
			m.pacman_food_collision.isMagic = it->isMagic;
			m.pacman_food_collision.isActive = it->isActive;

			_mngr->send(m);

			it->e->setAlive(false);
			it = _foods.erase(it);
		}
		else {
			++it;
		}
	}

}

