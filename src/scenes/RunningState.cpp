#include "RunningState.h"
#include "../game/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../utils/Collisions.h"
#include "../ecs/EntityManager.h"
#include "../components/Transform.h"
#include "../components/Health.h"
#include "../components/Image.h"
#include "../sdlutils/Texture.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/ImmunitySystem.h"

RunningState::RunningState() :
    _pacmanSys(nullptr), //
    _gameCtrlSys(nullptr), //
    _ghostSys(nullptr), //
    _foodSys(nullptr), //
    _immunitySys(nullptr), //
    _renderSys(nullptr), //
    _collisionSys(nullptr)
{

    auto* mngr = Game::Instance()->getMngr();

    _pacmanSys = mngr->addSystem<PacManSystem>();
    _foodSys = mngr->addSystem<FoodSystem>();
    _ghostSys = mngr->addSystem<GhostSystem>();
    _immunitySys = mngr->addSystem<ImmunitySystem>();
    _gameCtrlSys = mngr->addSystem<GameCtrlSystem>();
    _renderSys = mngr->addSystem<RenderSystem>();
    _collisionSys = mngr->addSystem<CollisionsSystem>();
}

void RunningState::enter() {
}

void RunningState::leave() {}

void RunningState::update() {
    updateSystems();

    // if press P, pause
    checkPause();
}

void
RunningState::updateSystems() {
    _pacmanSys->update();
    _foodSys->update();
    _ghostSys->update();
    _immunitySys->update();
    _gameCtrlSys->update();
    _collisionSys->update();
    _renderSys->update();
}

void
RunningState::checkPause() {
    if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_P)) {
        Game::Instance()->setState(Game::PAUSED);
        return;
    }
}