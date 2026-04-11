// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"
#include "../scenes/PausedState.h"
#include "../scenes/NewGameState.h"
#include "../scenes/NewRoundState.h"
#include "../scenes/RunningState.h"
#include "../scenes/GameOverState.h"

using ecs::EntityManager;

Game::Game() :
		_mngr(nullptr) {
}

Game::~Game() {
	delete _mngr;

	delete _paused_state;
	delete _runing_state;
	delete _newgame_state;
	delete _newround_state;
	delete _gameover_state;

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {

	// initialize the SDL singleton
	if (!SDLUtils::Init("PacMan", 800, 600,
			"resources/config/resources.json")) {

		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return false;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return false;

	}

	return true;
}

void Game::start() {

	// Create the manager
	_mngr = new EntityManager();

	// create the scenes
	_paused_state = new PausedState();
	_runing_state = new RunningState();
	_newgame_state = new NewGameState();
	_newround_state = new NewRoundState();
	_gameover_state = new GameOverState();

	setState(State::NEWGAME);

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();
	auto& vt = sdlutils().virtualTimer(); // virutal timer to be able to pause the timer in PausedState	

	vt.resetTime(); // start counting from this instance


	vt.pause(); // pause time to make sure no time passes until we start the game

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();
		vt.regCurrTime(); // register current time so all objects use the same time

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		_mngr->flush();
		sdlutils().clearRenderer();
		_state->update(); // update current state
		sdlutils().presentRenderer();

		_mngr->refresh();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

