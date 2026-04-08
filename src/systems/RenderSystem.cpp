// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {
}

void RenderSystem::initSystem() {
	_pacmanRender.srcRect = { PACMAN_SRC_POS, PACMAN_SRC_POS, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

	_ghostRender.srcRect = { 0, NORMAL_GHOST_SRC_Y, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

}

void RenderSystem::update() {
	drawMsgs();
	drawStars();
	drawPacMan();
}

void RenderSystem::drawStars() {
	// draw stars
	for (auto e : _mngr->getEntities(ecs::grp::GHOSTS)) {
		auto tr = _mngr->getComponent<Transform>(e);
		auto tex = _mngr->getComponent<Image>(e)->_tex;
		SDL_FRect srcRect = { _ghostRender.srcRect.x * _ghostRender.frame, _ghostRender.srcRect.y, _ghostRender.srcRect.w, _ghostRender.srcRect.h };

		draw(tr, tex, srcRect);
	}
}

void RenderSystem::drawPacMan() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto tr = _mngr->getComponent<Transform>(e);
	auto tex = _mngr->getComponent<Image>(e)->_tex;

	std::cout << _pacmanRender.frame << '\n';
	SDL_FRect srcRect = { _pacmanRender.srcRect.x + _pacmanRender.frame * _pacmanRender.srcRect.w, _pacmanRender.srcRect.y, _pacmanRender.srcRect.w, _pacmanRender.srcRect.h };
	draw(tr, tex, srcRect);

}

void RenderSystem::drawMsgs() {
	// draw the score
	//
	auto score = _mngr->getSystem<GameCtrlSystem>()->getScore();

	Texture scoreTex(sdlutils().renderer(), std::to_string(score),
			sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x444444ff));

	SDL_FRect dest = build_sdlfrect( //
			(sdlutils().width() - scoreTex.width()) / 2.0f, //
			10.0f, //
			scoreTex.width(), //
			scoreTex.height());

	scoreTex.render(dest);

	// draw add stars message
	sdlutils().msgs().at("addstars").render(10, 10);

}

void RenderSystem::draw(Transform *tr, const Texture *tex) {
	SDL_FRect dest = build_sdlfrect(tr->_pos, tr->_width, tr->_height);

	assert(tex != nullptr);
	tex->render(dest, tr->_rot);
}

void RenderSystem::draw(Transform* tr, const Texture* tex, SDL_FRect src) {
	SDL_FRect dest = build_sdlfrect(tr->_pos, tr->_width, tr->_height);

	assert(tex != nullptr);
	tex->render(src, dest, tr->_rot);
}