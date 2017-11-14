/*
 * Renderer.h
 *
 *  Created on: Nov 12, 2017
 *      Author: rbbender
 */

#ifndef INCLUDE_RENDERER_H_
#define INCLUDE_RENDERER_H_

#include <cstdio>
#include <cstdlib>
#include <controls.h>

class field_t;

class Renderer {
	field_t* pField;
    size_t szFldBlkX, szFldBlkY; // field size in blocks
public:
	Renderer(field_t* pFld): pField(pFld), szFldBlkX(pField->get_field_size_x()),
		szFldBlkY(pField->get_field_size_y())
		{};
	int render() = 0;
	int process_input() = 0;
	int init() = 0;
};

#endif /* INCLUDE_RENDERER_H_ */
