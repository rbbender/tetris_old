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

class Game;

class Renderer {
protected:
	Game* pGame;
    unsigned short szFldBlkX, szFldBlkY; // field size in blocks
    const unsigned short FLD_VIS_Y;
public:
	Renderer();
	virtual int process_input() = 0;
	virtual int init(Game*) = 0;
	virtual int render(double) = 0;
	virtual ~Renderer();
};

#endif /* INCLUDE_RENDERER_H_ */
