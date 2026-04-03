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

			Food f;
			f.e = e;
			f.isMagic = isMagic;
			f.isActive = false;
			f.timer = 0.0f;
			//f.nextChange = 10 + rand() % 11;

			if (isMagic) {
				f.nextChange = 10 + rand() % 11; // n 10 - 20 s
				_mngr->addComponent<Image>(e, &sdlutils().images().at("cherry"));// comienza en estado cereza
			}
			else{
				f.nextChange = 0.0f; // No aplica
				_mngr->addComponent<Image>(e, &sdlutils().images().at("cherry"));
			}


			_foods.push_back(f);
		}
	}
}

void FoodSystem::update() {
	for (auto& f : _foods) {

		if (!f.isMagic) continue;

		f.timer += sdlutils().deltaTime();

		if (f.timer >= f.nextChange) {

			f.timer = 0.0f;

			if (!f.isActive) {
				// cambia a estado milagro
				f.isActive = true;
				f.nextChange = 1 + rand() % 5;// m 1-5 s

			}
			else {
				// Vuelve a estado normal
				f.isActive = false;
				f.nextChange = 10 + rand() % 11;// n 10-20 s
			}
		}
	}

	// Comprobar colisiones con pacman
	auto pmTr = _mngr->getComponent<Transform>(_pacman);
	if (!pmTr) return;
	for (auto it = _foods.begin(); it != _foods.end(); ) {

		auto tr = _mngr->getComponent<Transform>(it->e);

		bool coll =
			tr->_pos.getX() < pmTr->_pos.getX() + pmTr->_width &&
			tr->_pos.getX() + tr->_width > pmTr->_pos.getX() &&
			tr->_pos.getY() < pmTr->_pos.getY() + pmTr->_height &&
			tr->_pos.getY() + tr->_height > pmTr->_pos.getY();

		if (coll) {
			// Fruta milagrosa activa y pacman no esta inmune ya
			if (it->isMagic && it->isActive && !_pacmanImmune) {

				_pacmanImmune = true;
				_immunityTimer = 10.0f;

				/*Message m;
				m.id = _m_IMMUNITY_START;
				_mngr->send(m);*/

				Message m;
				m.id = _m_PACMAN_FOOD_COLLISION;
				m.pacman_food_collision.isMagic = it->isMagic;
				m.pacman_food_collision.isActive = it->isActive;

				_mngr->send(m);
			}

			it->e->setAlive(false);
			it = _foods.erase(it);

			continue;
		}

		++it;
	}

	////temporizador de inmunidad
	//if (_pacmanImmune) {

	//	_immunityTimer -= sdlutils().deltaTime();

	//	if (_immunityTimer <= 0.0f) {
	//		_pacmanImmune = false;

	//		Message m;
	//		m.id = _m_IMMUNITY_END;
	//		_mngr->send(m);
	//	}
	//}

	if (_foods.empty()) {
		Message m;
		m.id = _m_GAME_OVER;
		_mngr->send(m);
	}
}

void FoodSystem::recieve(const Message& m) {

	switch (m.id) 
	{

	case _m_NEW_GAME:
		for (auto& f : _foods)
			f.e->setAlive(false);

		_foods.clear();
		initSystem();
		break;

	case _m_ROUND_START:
		for (auto& f : _foods) {
			f.timer = 0;
		}
		break;
	}
}
