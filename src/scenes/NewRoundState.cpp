#include "NewRoundState.h"
#include "../game/Game.h"
#include "../sdlutils/InputHandler.h"

NewRoundState::NewRoundState() {
    _tex = &sdlutils().msgs().at("new_round");
}

void NewRoundState::update() {
    assert(_tex != nullptr);

    _tex->render(sdlutils().width() / 2 - _tex->width() / 2, sdlutils().height() / 2 - _tex->height() / 2);

    if (ih().isKeyDown(SDL_SCANCODE_RETURN)) {
        Message m;
        m.id = _m_ROUND_START;
        Game::Instance()->getMngr()->send(m);
        Game::Instance()->setState(Game::RUNNING);
    }
}
