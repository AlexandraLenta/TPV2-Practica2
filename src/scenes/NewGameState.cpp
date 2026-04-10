#include "NewGameState.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewGameState::NewGameState() {
    _tex = &sdlutils().msgs().at("game_start");
}

void NewGameState::enter() {
    sdlutils().soundEffects().at("pacman_intro").play("se");
}

void NewGameState::update() {
    assert(_tex != nullptr);

    _tex->render((sdlutils().width() - _tex->width()) / 2, (sdlutils().height() - _tex->height()) / 2);

    if (ih().keyDownEvent()) {
        Game::Instance()->setState(Game::NEWROUND);
    }
}