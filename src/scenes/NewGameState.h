#pragma once
#include "GameState.h"
#include "../sdlutils/Texture.h"

class NewGameState :public GameState
{
public:
    NewGameState();
    virtual ~NewGameState() {}

    void update() override;
    void enter() override;
    void leave() override {}

private:
    const Texture* _tex;
};

