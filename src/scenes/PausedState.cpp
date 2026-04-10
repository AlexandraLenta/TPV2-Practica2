#include "PausedState.h"
#include "../game/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include <limits>

void PausedState::enter() {
    sdlutils().virtualTimer().pause(); // pause

    _tex = &sdlutils().msgs().at("pause"); // get pause text
}

void PausedState::leave() {
    sdlutils().virtualTimer().resume();
}

void PausedState::update() {
    _tex->render((sdlutils().width() - _tex->width()) / 2, (sdlutils().height() - _tex->height()) / 2);

    if (ih().keyDownEvent()) {
        Game::Instance()->setState(Game::RUNNING);
    }
}