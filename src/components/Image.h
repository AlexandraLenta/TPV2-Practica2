// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Texture;

struct Image: public ecs::Component {

	Image() :
			_tex() {
	}

	Image(const Texture *tex) :
			_tex(tex) {
	}

	Image(const Texture* tex, int row, int col) : _tex(tex), _texRow(row), _texCol(col) {

	}

	virtual ~Image() {
	}

	const Texture *_tex;
	int _texRow;
	int _texCol;
};

struct FramedImage : public Image {
	FramedImage() : _frames(1), _currFrame(0) {}

	FramedImage(const Texture *tex, int frames, int row, int col) : Image(tex, row, col), _frames(frames), _currFrame(0) {}

	const int _frames;
	int _currFrame;
};