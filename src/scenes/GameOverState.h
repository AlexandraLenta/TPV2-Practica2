#pragma once
#include "GameState.h"
#include "../sdlutils/Texture.h"

class GameOverState :
    public GameState
{
public:
    GameOverState() {};

    void enter() override;
    void update() override;
    void leave() override {}
private:
    const Texture* _tex;
};

