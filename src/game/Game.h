// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../utils/Singleton.h"
#include "../ecs/ecs.h"
#include "../ecs/EntityManager.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/ImmunitySystem.h"

class Game :public Singleton<Game> {
	friend Singleton<Game>;
public:
	Game();
	virtual ~Game();
	void init();
	void start();



private:
	ecs::EntityManager *_mngr;

	PacManSystem* _pacmanSys;
	GameCtrlSystem* _gameCtrlSys;
	GhostSystem* _ghostSys;
	FoodSystem* _foodSys;
	ImmunitySystem* _immunitySys;
	RenderSystem* _renderSys;
	CollisionsSystem* _collisionSys;



};

