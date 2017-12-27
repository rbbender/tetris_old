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
	bool flag_full_redraw;
	Game* pGame;
    unsigned short szFldBlkX, szFldBlkY; // field size in blocks
    unsigned short FLD_VIS_Y;
public:
	Renderer();
	bool is_full_redraw_required();
	void set_flag_full_redraw();
	void unset_flag_full_redraw();
	virtual int process_input() = 0;
	virtual int init(Game*);
	virtual int render(double) = 0;
	virtual int wrap_up();
	virtual ~Renderer();
};

#endif /* INCLUDE_RENDERER_H_ */
