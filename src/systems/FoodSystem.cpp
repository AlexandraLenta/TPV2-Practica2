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
	int rows = sdlutils().height() / FOOD_SIZE;
	int cols = sdlutils().width() / FOOD_SIZE;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			auto e = _mngr->addEntity();

			float x = GRID_SPACING + j * FOOD_SIZE;
			float y = GRID_SPACING + i * FOOD_SIZE;

			auto tr = _mngr->addComponent<Transform>(e);
			tr->init(Vector2D(x, y), Vector2D(), FOOD_SIZE, FOOD_SIZE, 0);

			bool isMagic = (rand() % 100) < 10;

			Food f;
			f.e = e;
			f.isMagic = isMagic;
			f.isActive = false;
			f.lastChangeTime = sdlutils().virtualTimer().currTime();
			f.activeTime = 0.0f;

			if (isMagic)
				f.activeFrecuency = 10 + rand() % 11;
			else
				f.activeFrecuency = 0;

			auto img = _mngr->addComponent<Image>(e);
			img->_tex = &sdlutils().images().at("pacman");

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
		m.game_over_data.hasWon = true;
		_mngr->send(m);
	}
}

void FoodSystem::updateMagicState() {

	auto& vT = sdlutils().virtualTimer();

	for (auto& f : _foods) {

		if (!f.isMagic) continue;

		if (!f.isActive) {
			if (vT.currTime() - f.lastChangeTime >= f.activeFrecuency) {
				f.isActive = true;
				f.activeTime = 1 + rand() % 5;
				f.lastChangeTime = vT.currTime();
			}
		}
		else {
			if (vT.currTime() - f.lastChangeTime >= f.activeTime) {
				f.isActive = false;
				f.lastChangeTime = vT.currTime();
			}
		}
	}
}

void FoodSystem::recieve(const Message& m) {

	switch (m.id) 
	{
		case _m_NEW_GAME:
			for (auto& f : _foods)
				f.e->setAlive(true);
				// reset all timers
			break;

		case _m_ROUND_START:
			for (auto& f : _foods) {
				// reset timers
			}
			break;
	}
}
