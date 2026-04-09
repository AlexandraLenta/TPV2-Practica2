#include "FoodSystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"

FoodSystem::FoodSystem() {

}

FoodSystem::~FoodSystem() {

}

void FoodSystem::initSystem() {
	int rows = 6;
	int cols = 8;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			auto e = _mngr->addEntity(ecs::grp::FRUIT);

			float x = MARGIN + j * (FOOD_SIZE + GRID_SPACING);
			float y = MARGIN + i * (FOOD_SIZE + GRID_SPACING);

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
				f.activeFrecuency = 10000 + rand() % 11000;
			else
				f.activeFrecuency = 0;

			std::cout << f.activeFrecuency << '\n';

			auto img = _mngr->addComponent<Image>(e, &sdlutils().images().at("pacman"), FRUIT_ROW, FRUIT_NORMAL_COL);

			_foods.push_back(f);
		}
	}
}

void FoodSystem::update() {

	updateMagicState();

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
				f.activeTime = (1 + rand() % 5) * 1000.0f;
				f.lastChangeTime = vT.currTime();
				auto img = _mngr->getComponent<Image>(f.e)->_texCol = FRUIT_MAGIC_COL;
			}
		}
		else {
			if (vT.currTime() - f.lastChangeTime >= f.activeTime) {
				f.isActive = false;
				f.lastChangeTime = vT.currTime();
				auto img = _mngr->getComponent<Image>(f.e)->_texCol = FRUIT_NORMAL_COL;
			}
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
				f.isActive = false;
				f.lastChangeTime = sdlutils().virtualTimer().currTime();
			}
			break;
	}
}
