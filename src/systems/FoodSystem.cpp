#include "FoodSystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"

FoodSystem::FoodSystem() : _size(30.0f),_spacing(80.0f) {

}

FoodSystem::~FoodSystem() {

}

void FoodSystem::initSystem() {

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

			if (isMagic)
				f.nextChange = 10 + rand() % 11;
			else
				f.nextChange = 0;

			auto img = _mngr->addComponent<Image>(e);
			img->_tex = &sdlutils().images().at("cherry");

			_foods.push_back(f);
		}
	}
}

void FoodSystem::update() {

	updateMagicState();
	checkCollisions();

	if (_foods.empty()) {
		Message m;
		m.id = _m_GAME_OVER;
		_mngr->send(m);
	}
}

void FoodSystem::updateMagicState() {

	float dt = sdlutils().deltaTime() / 1000.0f;

	for (auto& f : _foods) {

		if (!f.isMagic) continue;

		f.timer += dt;

		if (f.timer >= f.nextChange) {

			f.timer = 0.0f;

			auto img = _mngr->getComponent<Image>(f.e);

			if (!f.isActive) {
				f.isActive = true;
				f.nextChange = 1 + rand() % 5;

				if (img)
					img->_tex = &sdlutils().images().at("pear");

			}
			else {
				f.isActive = false;
				f.nextChange = 10 + rand() % 11;

				if (img)
					img->_tex = &sdlutils().images().at("cherry");
			}
		}
	}
}

void FoodSystem::checkCollisions() {

	auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTr = _mngr->getComponent<Transform>(pacman);

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
