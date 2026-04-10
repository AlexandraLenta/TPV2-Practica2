#pragma once
#include "GameState.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"

class RunningState :
    public GameState
{

public:
    RunningState();
    virtual ~RunningState() {}

    void enter() override;
    void leave() override;
    void update() override;

private:
    void checkPause();
    void updateSystems();

    PacManSystem* _pacmanSys;
    GameCtrlSystem* _gameCtrlSys;
    GhostSystem* _ghostSys;
    FoodSystem* _foodSys;
    ImmunitySystem* _immunitySys;
    RenderSystem* _renderSys;
    CollisionsSystem* _collisionSys;
};

