#include "GhostSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrlSystem.h"

GhostSystem::GhostSystem() {

}

GhostSystem::~GhostSystem() {

}

void GhostSystem::initSystem() {
	_pacman = _mngr->getHandler(ecs::hdlr::PACMAN);

	_spawnTimer = 0.0f;
	_pacmanImmune = false;
}