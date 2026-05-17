#include "GhostSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Immunity.h"
#include "../components/GhostInfo.h"
#include "../components/Health.h"
#include "../game/Game.h"

void GhostSystem::initSystem() {
	_pacman = _mngr->getHandler(ecs::hdlr::PACMAN);

	auto& vT = sdlutils().virtualTimer();
	_previousSpawnTime = vT.currTime();

	_lastFrameChange = vT.currTime();
}

void GhostSystem::update() {
	auto& vT = sdlutils().virtualTimer();

	// Spawn ghost
	if (vT.currTime() - _previousSpawnTime >= SPAWN_INTERVAL) {
		_previousSpawnTime = vT.currTime(); // restart timer

		if (_mngr->getEntities(ecs::grp::GHOSTS).size() < 10 && !_mngr->getComponent<Immunity>(_pacman)->_isImmune) { // check conditions: less than 10 ghosts and pacman not immune			
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


			// weak ghost
			bool isWeak = (rand() % 100) < 50;

			int hits = 0;

			if (isWeak)
				hits = 1 + rand() % 5;

			_mngr->addComponent<GhostInfo>(g, isWeak, hits, Vector2D(x, y));

			_mngr->addComponent<FramedImage>(g, &sdlutils().images().at("pacman"), 8, isWeak ? PURPLE_GHOST_SRC_ROW : NORMAL_GHOST_SRC_ROW, 0);
			std::cout << "Hits: " << hits << '\n';
		}
	}

	// Update ghost movement
	for (auto& g : _mngr->getEntities(ecs::grp::GHOSTS)) {
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
		for (auto& g : _mngr->getEntities(ecs::grp::GHOSTS)) {
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
		for (auto g : _mngr->getEntities(ecs::grp::GHOSTS))
			_mngr->setAlive(g, false);
		break;

	case _m_IMMUNITY_START:
		for (auto& g : _mngr->getEntities(ecs::grp::GHOSTS)) {
			auto* img = _mngr->getComponent<FramedImage>(g);
			img->_texRow = BLUE_GHOST_SRC_ROW;
		}
		break;

	case _m_IMMUNITY_END:
		for (auto& g : _mngr->getEntities(ecs::grp::GHOSTS)) {
			auto* img = _mngr->getComponent<FramedImage>(g);
			img->_texRow = NORMAL_GHOST_SRC_ROW;
		}
		break;

	case _m_PACMAN_GHOST_COLLISION:
		auto& ghost = m.ghost_collision_data.e;
		if (_mngr->getComponent<Immunity>(_pacman)->_isImmune) {

			sdlutils().soundEffects().at("pacman_chomp").play("se");
			_mngr->setAlive(ghost, false);
		}
		else {
			auto& ghost = m.ghost_collision_data.e;

			auto* ghostInfo = _mngr->getComponent<GhostInfo>(ghost);

			ghostInfo->_hitsLeft--;

			std::cout << "Hits left: " << ghostInfo->_hitsLeft << '\n';
			
			if (ghostInfo->_hitsLeft < 0) {

				auto* health = _mngr->getComponent<Health>(_pacman);
				health->_hp--;

				Message m;
				if (health->_hp <= 0) {
					m.id = _m_GAME_OVER;
					_mngr->send(m);

					Game::Instance()->setState(Game::State::GAMEOVER);
				}
				else {
					m.id = _m_ROUND_START;
					_mngr->send(m);

					Game::Instance()->setState(Game::State::NEWROUND);
				}
			}
			else {
				_mngr->getComponent<Transform>(ghost)->_pos = ghostInfo->_originalPos;
				if (ghostInfo->_hitsLeft == 0) {
					auto* img = _mngr->getComponent<FramedImage>(ghost);
					img->_texRow = NORMAL_GHOST_SRC_ROW;
				}
			}
		}

		break;
	}
}