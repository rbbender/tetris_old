/*
 * Render.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: rbbender
 */
#include <renderer/Renderer.h>
#include <Game.h>

Renderer::Renderer():
	flag_full_redraw(true),
	pGame(nullptr),
	szFldBlkX(0),
	szFldBlkY(0),
	FLD_VIS_Y(0)
	{};

Renderer::~Renderer() {};

int Renderer::init(Game* pGm) {
	pGame = pGm;
	auto pField = pGame->get_field();
	szFldBlkX = pField->get_field_size_x();
	szFldBlkY = pField->get_field_size_y();
	FLD_VIS_Y = pField->get_vis_y();
	return 0;
}

bool Renderer::is_full_redraw_required() {
	return flag_full_redraw;
}

void Renderer::set_flag_full_redraw() {
	flag_full_redraw = true;
}

void Renderer::unset_flag_full_redraw() {
	flag_full_redraw = false;
}

int Renderer::wrap_up() {
	pGame = nullptr;
	szFldBlkX = 0;
	szFldBlkY = 0;
	FLD_VIS_Y = 0;
	return 0;
}
