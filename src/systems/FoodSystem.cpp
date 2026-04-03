#include "FoodSystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"

FoodSystem::FoodSystem() {

}

FoodSystem::~FoodSystem() {

}

void FoodSystem::initSystem() {

	_pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
	_pacmanImmune = false;
	_immunityTimer = 0.0f;

	int rows = 5;
	int cols = 8;
	float spacing = 70.0f;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			auto e = _mngr->addEntity();

			float x = 50 + j * spacing;
			float y = 50 + i * spacing;

			auto tr = _mngr->addComponent<Transform>(e);
			tr->init(Vector2D(x, y), Vector2D(), 30, 30, 0);

			bool isMagic = (rand() % 100) < 10;

			if (isMagic)
				_mngr->addComponent<Image>(e, &sdlutils().images().at("cherry"));
			else
				_mngr->addComponent<Image>(e, &sdlutils().images().at("cherry"));

			Food f;
			f.e = e;
			f.isMagic = isMagic;
			f.isActive = false;
			f.timer = 0.0f;
			f.nextChange = 10 + rand() % 11;

			_foods.push_back(f);
		}
	}
}

void FoodSystem::update() {
	for (auto& f : _foods) {

		if (!f.isMagic) continue;

		f.timer += sdlutils().deltaTime() / 1000.0f;

		if (f.timer >= f.nextChange) {

			f.timer = 0.0f;

			if (!f.isActive) {
				f.isActive = true;
				f.nextChange = 1 + rand() % 5;

			}
			else {
				f.isActive = false;
				f.nextChange = 10 + rand() % 11;
			}
		}
	}

	auto pmTr = _mngr->getComponent<Transform>(_pacman);

	for (auto it = _foods.begin(); it != _foods.end(); ) {

		auto tr = _mngr->getComponent<Transform>(it->e);

		bool coll =
			tr->_pos.getX() < pmTr->_pos.getX() + pmTr->_width &&
			tr->_pos.getX() + tr->_width > pmTr->_pos.getX() &&
			tr->_pos.getY() < pmTr->_pos.getY() + pmTr->_height &&
			tr->_pos.getY() + tr->_height > pmTr->_pos.getY();

		if (coll) {
			if (it->isMagic && it->isActive && !_pacmanImmune) {

				_pacmanImmune = true;
				_immunityTimer = 10.0f;

				Message m;
				m.id = _m_IMMUNITY_START;
				_mngr->send(m);
			}

			it->e->setAlive(false);
			it = _foods.erase(it);

			continue;
		}

		++it;
	}

	if (_pacmanImmune) {

		_immunityTimer -= sdlutils().deltaTime() / 1000.0f;

		if (_immunityTimer <= 0.0f) {
			_pacmanImmune = false;

			Message m;
			m.id = _m_IMMUNITY_END;
			_mngr->send(m);
		}
	}
	if (_foods.empty()) {
		Message m;
		m.id = _m_GAME_OVER;
		_mngr->send(m);
	}
}

