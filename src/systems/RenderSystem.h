// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"
#include <SDL_image.h>

class Texture;
struct Transform;

class RenderSystem: public ecs::System {
public:

	RenderSystem();
	virtual ~RenderSystem();
	void initSystem() override;
	void update() override;
private:
	void drawStars();
	void drawPacMan();
	void drawMsgs();
	void draw(Transform *tr, const Texture *tex);
	void draw(Transform* tr, const Texture* tex, SDL_FRect src);

	struct renderInfo {
		int frame = 1;
		SDL_FRect srcRect;
	} _pacmanRender, _ghostRender;

	const float PACMAN_SRC_POS = 0;
	const float NORMAL_GHOST_SRC_Y = 200;
	const float BLUE_GHOST_SRC_Y = 300;
	const float SPRITE_SRC_SIZE = 128;
};

