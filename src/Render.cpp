/*
 * Render.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: rbbender
 */
#include <renderer/Renderer.h>
#include <field.h>

Renderer::Renderer(field_t* pFld):
	pField(pFld),
	szFldBlkX(pField->get_field_size_x()),
	szFldBlkY(pField->get_field_size_y()),
	FLD_VIS_Y(pField->get_vis_y())
	{};

Renderer::~Renderer() {};

