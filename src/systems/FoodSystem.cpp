#include "FoodSystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/FoodInfo.h"
#include "../game/Game.h"

FoodSystem::FoodSystem() : _pacman(nullptr) {

}

FoodSystem::~FoodSystem() {

}

void FoodSystem::createFood() {
	std::cout << "creating food...\n";
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

			float activeF = 0;

			if (isMagic)
				activeF = 10000 + rand() % 11000;

			auto foodComp = _mngr->addComponent<FoodInfo>(e, isMagic, sdlutils().virtualTimer().currTime(), activeF, 0.0f);

			auto img = _mngr->addComponent<Image>(e, &sdlutils().images().at("pacman"), FRUIT_ROW, FRUIT_NORMAL_COL);

		}
	}
}

void FoodSystem::initSystem() {
}

void FoodSystem::update() {

	updateMagicState();

	if (_mngr->getEntities(ecs::grp::FRUIT).size() <= 0) {
		std::cout << "EMPTY\n";
		Message m;
		m.id = _m_GAME_OVER;
		_mngr->send(m);
	}
}

void FoodSystem::updateMagicState() {

	auto& vT = sdlutils().virtualTimer();

	for (auto& e : _mngr->getEntities(ecs::grp::FRUIT)) {
		auto* f = _mngr->getComponent<FoodInfo>(e);

		if (!f->_isMagic) continue;

		if (!f->_isActive) {
			if (vT.currTime() - f->_lastChangeTime >= f->_activeFrecuency) {
				f->_isActive = true;
				f->_activeTime = (1 + rand() % 5) * 1000.0f;
				f->_lastChangeTime = vT.currTime();
				auto img = _mngr->getComponent<Image>(e)->_texCol = FRUIT_MAGIC_COL;
			}
		}
		else {
			if (vT.currTime() - f->_lastChangeTime >= f->_activeTime) {
				f->_isActive = false;
				f->_lastChangeTime = vT.currTime();
				auto img = _mngr->getComponent<Image>(e)->_texCol = FRUIT_NORMAL_COL;
			}
		}
	}
}

void FoodSystem::recieve(const Message& m) {
	auto& food = _mngr->getEntities(ecs::grp::FRUIT);
	switch (m.id) 
	{
		case _m_NEW_GAME:
			for (auto& f : food)
				_mngr->setAlive(f, false);
			
			createFood();

			break;

		case _m_ROUND_START:
			for (auto& f : food) {
				auto* fInfo = _mngr->getComponent<FoodInfo>(f);
				fInfo->_isActive = false;
				fInfo->_lastChangeTime = sdlutils().virtualTimer().currTime();
			}
			break;
		case _m_PACMAN_FOOD_COLLISION:
			std::cout << "Col:" << food.size() << '\n';
			_mngr->setAlive(m.food_collision_data.e, false); // remove fruit
			if (food.size() <= 1) { // we check -1 because the fruit is not removed till the end of the frame
				std::cout << "collision game over\n";
				Message m;
				m.id = _m_GAME_OVER;
				_mngr->send(m);

				Game::Instance()->setGameOver(true);
			}
			break;
	}
}
