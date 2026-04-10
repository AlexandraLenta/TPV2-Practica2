// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Health.h"
#include "../components/Immunity.h"
#include "../game/Game.h"

PacManSystem::PacManSystem() :
		_pmTR(nullptr) {
}

PacManSystem::~PacManSystem() {
}

void PacManSystem::initSystem() {
	// create the PacMan entity
	//
	_pacman = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::PACMAN, _pacman);

	_lastFrameChange = sdlutils().virtualTimer().currTime();

	_pmTR = _mngr->addComponent<Transform>(_pacman);
	auto s = 75.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;
	_pmTR->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	_mngr->addComponent<FramedImage>(_pacman, &sdlutils().images().at("pacman"), 2, PACMAN_SRC_ROW, PACMAN_SRC_COL);
	_mngr->addComponent<Immunity>(_pacman);
	_mngr->addComponent<Health>(_pacman, HP);

}

void PacManSystem::update() {

	auto &ihldr = ih();

	if (ihldr.keyDownEvent()) {

		if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT)) { // rotate right
			_pmTR->_rot = _pmTR->_rot + 90.0f;
			_pmTR->_vel = _pmTR->_vel.rotate(90.0f);

		} else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT)) { // rotate left
			_pmTR->_rot = _pmTR->_rot - 90.0f;
			_pmTR->_vel = _pmTR->_vel.rotate(-90.0f);

		} else if (ihldr.isKeyDown(SDL_SCANCODE_UP)) { // increase speed
			_pmTR->_vel = Vector2D(0.0f, -3.0f).rotate(_pmTR->_rot);

		} else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN)) { // stop
			_pmTR->_vel = Vector2D(0.0f, 0.0f);
		}

	}

	// move the pacman
	_pmTR->_pos = _pmTR->_pos + _pmTR->_vel;

	// check left/right borders
	if (_pmTR->_pos.getX() < 0) {
		_pmTR->_pos.setX(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	} else if (_pmTR->_pos.getX() + _pmTR->_width > sdlutils().width()) {
		_pmTR->_pos.setX(sdlutils().width() - _pmTR->_width);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	// check upper/lower borders
	if (_pmTR->_pos.getY() < 0) {
		_pmTR->_pos.setY(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	} else if (_pmTR->_pos.getY() + _pmTR->_height > sdlutils().height()) {
		_pmTR->_pos.setY(sdlutils().height() - _pmTR->_height);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	// advance frame
	if (sdlutils().virtualTimer().currTime() - _lastFrameChange >= FRAME_CHANGE) {
		_lastFrameChange = sdlutils().virtualTimer().currTime();
		auto* pFI = _mngr->getComponent<FramedImage>(_pacman);
		pFI->_currFrame++;

		if (pFI->_currFrame >= pFI->_frames) {
			pFI->_currFrame = 0;
		}
	}
}

void PacManSystem::recieve(const Message& m) {
	switch (m.id) 
	{
	case _m_ROUND_START:
		_pmTR->_pos.set((sdlutils().width() - _pmTR->_width) / 2.0f,(sdlutils().height() - _pmTR->_height) / 2.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
		break;

	case _m_NEW_GAME:
		_mngr->getComponent<Health>(_pacman)->_hp = HP;
		break;

	case _m_IMMUNITY_START:
		_mngr->getComponent<Immunity>(_pacman)->_isImmune = true;
		break;

	case _m_IMMUNITY_END:
		_mngr->getComponent<Immunity>(_pacman)->_isImmune = false;
		break;

	case _m_PACMAN_GHOST_COLLISION:
		if (!_mngr->getComponent<Immunity>(_pacman)->_isImmune) {
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
	}
}
