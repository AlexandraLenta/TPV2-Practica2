#include "GhostSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"

void GhostSystem::initSystem() {
	_pacman = _mngr->getHandler(ecs::hdlr::PACMAN);

	_spawnTimer = 0.0f;
	_pacmanImmune = false;
}

void GhostSystem::update() {
	_spawnTimer += sdlutils().deltaTime();

	if (_spawnTimer >= SPAWN_INTERVAL) {
		_spawnTimer = 0.0f; // restart timer

		if (_ghosts.size() < 10 && !_pacmanImmune) { // check conditions: less than 10 ghosts and pacman not immune
			auto g = _mngr->addEntity(); // create the entity

			// random corner
			// 0: up left           0 ____ 1
			// 1: up right           |    |
			// 2: down right       3 |____| 2
			// 3: down left
			int corner = rand() % 4;
			float x = (corner == 0 || corner == 3) ? 0 : sdlutils().width() - 50; 
			float y = (corner == 0 || corner == 1) ? 0 : sdlutils().height() - 50;

			_mngr->addComponent<Transform>(g)->init(Vector2D(x, y), Vector2D(0, 0), 50, 50, 0);
			_mngr->addComponent<Image>(g, &sdlutils().images().at("ghost"));
			_ghosts.push_back(g);
		}
	}


	for (auto g :_ghosts) {
		auto tr = _mngr->getComponent<Transform>(g);

		if ((rand() / (float)RAND_MAX) < 0.005f) {
			Vector2D dir = (_mngr->getComponent<Transform>(_pacman)->_pos - tr->_pos).normalize();
			tr->_vel = dir * 1.1f;
		}
		tr->_pos = tr->_pos + tr->_vel;

		// rebot
		if (tr->_pos.getX() <= 0 || tr->_pos.getX() + tr->_width >= sdlutils().width()) {
			tr->_vel.setX(-tr->_vel.getX());
		}
		if (tr->_pos.getY() <= 0 || tr->_pos.getY() + tr->_height >= sdlutils().height())
			tr->_vel.setY(-tr->_vel.getY());
	}

	for (auto it = _ghosts.begin(); it != _ghosts.end(); ) {

		auto tr = _mngr->getComponent<Transform>(*it);
		auto pmTr = _mngr->getComponent<Transform>(_pacman);

		bool coll =tr->_pos.getX() < pmTr->_pos.getX() + pmTr->_width && tr->_pos.getX() + tr->_width > pmTr->_pos.getX() &&
			tr->_pos.getY() < pmTr->_pos.getY() + pmTr->_height && tr->_pos.getY() + tr->_height > pmTr->_pos.getY();

		if (coll) {
			if (_pacmanImmune) {
				_mngr->setAlive(*it,false);
				it = _ghosts.erase(it);
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
}

void GhostSystem::recieve(const Message& m) {

	switch (m.id) {

	case _m_ROUND_START:
	case _m_ROUND_OVER:
		for (auto g : _ghosts)
			_mngr->setAlive(g, false);
		_ghosts.clear();
		break;

	case _m_IMMUNITY_START:
		_pacmanImmune = true;
		break;

	case _m_IMMUNITY_END:
		_pacmanImmune = false;
		break;
	}
}