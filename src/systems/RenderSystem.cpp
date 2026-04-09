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
	auto img = _mngr->getComponent<FramedImage>(e);
	auto tex = img->_tex;

	SDL_FRect src = { 0, img->_texRow * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

	SDL_FRect srcRect = { src.x + img->_currFrame * src.w, src.y, src.w, src.h };
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
	auto ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	for (auto& g : ghosts) {

		auto tr = _mngr->getComponent<Transform>(g);
		auto img = _mngr->getComponent<FramedImage>(g);
		auto tex = img->_tex;

		SDL_FRect src = { 0, img->_texRow * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

		SDL_FRect srcRect = { src.x + img->_currFrame * src.w, src.y, src.w, src.h };
		draw(tr, tex, srcRect);
	}
}

void
RenderSystem::drawFood() {
	auto food = _mngr->getEntities(ecs::grp::FRUIT);
	for (auto& f : food) {
		auto tr = _mngr->getComponent<Transform>(f);
		auto img = _mngr->getComponent<Image>(f);
		auto tex = img->_tex;

		SDL_FRect src = { img->_texCol * SPRITE_SRC_SIZE, img->_texRow * SPRITE_SRC_SIZE, SPRITE_SRC_SIZE, SPRITE_SRC_SIZE };

		SDL_FRect srcRect = { src.x, src.y, src.w, src.h };
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