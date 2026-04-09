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
	_pacmanRender.srcRect = { 0, PACMAN_SRC_ROW * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

	_ghostRender.srcRect = { 0, NORMAL_GHOST_SRC_ROW * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

	_foodRender.frame = 0;
	_foodRender.srcRect = { FRUIT_NORMAL_COL * SPRITE_SRC_SIZE, FRUIT_ROW * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };
}

void RenderSystem::update() {
	drawMsgs();
	drawPacMan();
	drawGhosts();
	drawFood();
}

void RenderSystem::drawPacMan() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto tr = _mngr->getComponent<Transform>(e);
	auto tex = _mngr->getComponent<Image>(e)->_tex;

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
}

void
RenderSystem::drawGhosts() {
	auto ghosts= _mngr->getEntities(ecs::grp::GHOSTS);
	for (auto& g : ghosts) {

		auto tr = _mngr->getComponent<Transform>(g);
		auto tex = _mngr->getComponent<Image>(g)->_tex;

		SDL_FRect srcRect = { _ghostRender.srcRect.x + _ghostRender.frame * _ghostRender.srcRect.w, _ghostRender.srcRect.y, _ghostRender.srcRect.w, _ghostRender.srcRect.h };
		draw(tr, tex, srcRect);
	}
}

void
RenderSystem::drawFood() {
	auto food = _mngr->getEntities(ecs::grp::FRUIT);
	for (auto& f : food) {
		auto tr = _mngr->getComponent<Transform>(f);
		auto tex = _mngr->getComponent<Image>(f)->_tex;

		SDL_FRect srcRect = { _foodRender.srcRect.x + _foodRender.frame * _foodRender.srcRect.w, _foodRender.srcRect.y, _foodRender.srcRect.w, _foodRender.srcRect.h };
		draw(tr, tex, srcRect);
	}
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

void
RenderSystem::recieve(const Message& m) {
	switch (m.id) {
	case _m_IMMUNITY_START:
		_ghostRender.srcRect = { 0, BLUE_GHOST_SRC_ROW * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };
		break;
	case _m_IMMUNITY_END:
		_ghostRender.srcRect = { 0, NORMAL_GHOST_SRC_ROW * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };
		break;
	}
}