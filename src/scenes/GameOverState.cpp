#include "GameOverState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

void GameOverState::enter() {
	if (Game::Instance()->getMngr()->getEntities(ecs::grp::FRUIT).size() <= 0) {
		_tex = &sdlutils().msgs().at("game_over_win");
		sdlutils().soundEffects().at("pacman_won").play("se");
	}
	else {
		_tex = &sdlutils().msgs().at("game_over_lose");
		sdlutils().soundEffects().at("pacman_death").play("se");
	}
}

void GameOverState::update() {
	assert(_tex != nullptr);

	_tex->render((sdlutils().width() - _tex->width()) / 2, (sdlutils().height() - _tex->height()) / 2);

	if (ih().isKeyDown(SDL_SCANCODE_RETURN)) {
		Game::Instance()->setState(Game::NEWGAME);
	}
}