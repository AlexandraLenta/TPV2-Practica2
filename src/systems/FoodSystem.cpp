#include "FoodSystem.h"
#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/FoodInfo.h"
#include "GameCtrlSystem.h"
#include "../game/Game.h"

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

			float activeF = 0;

			if (isMagic)
				activeF = 10000 + rand() % 11000;

			auto foodComp = _mngr->addComponent<FoodInfo>(e, isMagic, sdlutils().virtualTimer().currTime(), activeF, 0.0f);

			auto img = _mngr->addComponent<Image>(e, &sdlutils().images().at("pacman"), FRUIT_ROW, FRUIT_NORMAL_COL);

			_foods.push_back(e);
		}
	}
}

void FoodSystem::update() {

	updateMagicState();

	if (_foods.empty()) {
		Message m;
		m.id = _m_GAME_OVER;
		_mngr->send(m);
	}
}

void FoodSystem::updateMagicState() {

	auto& vT = sdlutils().virtualTimer();

	for (auto& e : _foods) {
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

	switch (m.id) 
	{
		case _m_NEW_GAME:
			for (auto& f : _foods)
				f->setAlive(false);
			
			_foods.clear();
			initSystem();
				
			break;

		case _m_ROUND_START:
			for (auto& f : _foods) {
				auto* fInfo = _mngr->getComponent<FoodInfo>(f);
				fInfo->_isActive = false;
				fInfo->_lastChangeTime = sdlutils().virtualTimer().currTime();
			}
			break;
		case _m_PACMAN_FOOD_COLLISION:
			_mngr->setAlive(m.food_collision_data.e, false); // remove fruit
			_foods.erase(std::find(_foods.begin(), _foods.end(), m.food_collision_data.e));
			if (_foods.size() <= 0) {
				Message m;
				m.id = _m_GAME_OVER;
				_mngr->send(m);

				Game::Instance()->setState(Game::State::GAMEOVER);
				// set win or lose
			}
			break;
	}
}
