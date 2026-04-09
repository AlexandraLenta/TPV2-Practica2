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

	auto& vT = sdlutils().virtualTimer();
	_previousSpawnTime = vT.currTime();
	_pacmanImmune = false;

	_lastFrameChange = vT.currTime();
}

void GhostSystem::update() {
	auto& vT = sdlutils().virtualTimer();

	// Spawn ghost
	if (vT.currTime() - _previousSpawnTime >= SPAWN_INTERVAL) {
		_previousSpawnTime = vT.currTime(); // restart timer

		if (_ghosts.size() < 10 && !_pacmanImmune) { // check conditions: less than 10 ghosts and pacman not immune			
			auto g = _mngr->addEntity(ecs::grp::GHOSTS); // create the entity
			
			// random corner
			// 0: up left           0 ____ 1
			// 1: up right           |    |
			// 2: down right       3 |____| 2
			// 3: down left
			int corner = rand() % 4;
			float x = (corner == 0 || corner == 3) ? 0 : sdlutils().width() - GHOST_SIZE;
			float y = (corner == 0 || corner == 1) ? 0 : sdlutils().height() - GHOST_SIZE;

			_mngr->addComponent<Transform>(g)->init(Vector2D(x, y), Vector2D(0, 0), GHOST_SIZE, GHOST_SIZE, 0);
			_mngr->addComponent<FramedImage>(g, &sdlutils().images().at("pacman"), 8, NORMAL_GHOST_SRC_ROW, 0);
			_ghosts.push_back(g);

		}
	}

	// Update ghost movement
	for (auto& g :_ghosts) {
		auto tr = _mngr->getComponent<Transform>(g);

		// Follow pacman
		if ((rand() % 1000) < UPDATE_VEL_PROBABILITY * 1000) {
			Vector2D dir = (_mngr->getComponent<Transform>(_pacman)->_pos - tr->_pos).normalize();
			tr->_vel = dir * 1.1f;
		}

		// Update pos
		tr->_pos = tr->_pos + tr->_vel;

		// Border check
		if (tr->_pos.getX() <= 0 || tr->_pos.getX() + tr->_width >= sdlutils().width()) {
			tr->_vel.setX(-tr->_vel.getX());
		}
		if (tr->_pos.getY() <= 0 || tr->_pos.getY() + tr->_height >= sdlutils().height())
			tr->_vel.setY(-tr->_vel.getY());
	}

	// advance frame
	if (sdlutils().virtualTimer().currTime() - _lastFrameChange >= FRAME_CHANGE_INTERVAL) {
		_lastFrameChange = sdlutils().virtualTimer().currTime();
		for (auto& g : _ghosts) {
			auto* gFI = _mngr->getComponent<FramedImage>(g);
			gFI->_currFrame++;

			if (gFI->_currFrame >= gFI->_frames) {
				gFI->_currFrame = 0;
			}
		}
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
		for (auto& g : _ghosts) {
			auto* img = _mngr->getComponent<FramedImage>(g);
			img->_texCol = BLUE_GHOST_SRC_ROW;
		}
		break;

	case _m_IMMUNITY_END:
		_pacmanImmune = false;
		for (auto& g : _ghosts) {
			auto* img = _mngr->getComponent<FramedImage>(g);
			img->_texCol = NORMAL_GHOST_SRC_ROW;
		}
		break;

	case _m_PACMAN_GHOST_COLLISION:
		if (_pacmanImmune) {
			_mngr->setAlive(m.ghost_collision_data.e, false);
			_ghosts.erase(std::find(_ghosts.begin(), _ghosts.end(), m.ghost_collision_data.e));
		}
		else {
			std::cout << "pacman dead.\n";
		}
		break;
	}
}