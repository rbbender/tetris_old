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
#include <sstream>
#include <controls.h>

class field_t;

class Renderer {
protected:
	field_t* pField;
    unsigned short szFldBlkX, szFldBlkY; // field size in blocks
    const unsigned short FLD_VIS_Y;
public:
	Renderer(field_t* pFld);
	virtual int process_input() = 0;
	virtual int init() = 0;
	virtual int render(double) = 0;
	virtual ~Renderer();
};

#endif /* INCLUDE_RENDERER_H_ */
