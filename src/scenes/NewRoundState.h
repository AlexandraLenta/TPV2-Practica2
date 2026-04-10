#pragma once
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"

class AsteroidsUtils;
class FighterFacade;

class NewRoundState :
    public GameState
{
public:
    NewRoundState();
    virtual ~NewRoundState() {}

    void update() override;
    void enter() override {}
    void leave() override {}

private:
    const Texture* _tex;
};

