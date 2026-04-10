#pragma once
#include "GameState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/Font.h"

class PausedState :
    public GameState
{
public:
    PausedState() {}
    virtual ~PausedState() {}

    void enter() override;
    void leave() override;
    void update() override;

private:
    const Texture* _tex;
};

