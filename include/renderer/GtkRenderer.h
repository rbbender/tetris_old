/*
 * GtkDrawingAreaRenderer.h
 *
 *  Created on: Dec 11, 2017
 *      Author: rbbender
 */

#ifndef INCLUDE_RENDERER_GTKRENDERER_H_
#define INCLUDE_RENDERER_GTKRENDERER_H_
#include "renderer/Renderer.h"
#include <gtkmm/builder.h>
#include <gtkmm/applicationwindow.h>

class GtkRenderer: public Renderer {
private:
	const Glib::RefPtr<Gtk::Builder>& gtkBuilder;
	Gtk::ApplicationWindow* pGtkMainWnd;
public:
	GtkRenderer(field_t* fld, const Glib::RefPtr<Gtk::Builder>& bld);
	virtual ~GtkRenderer();
	virtual int process_input();
	virtual int render(double);
	virtual int init();
	Gtk::ApplicationWindow* get_mainwindow();
};



#endif /* INCLUDE_RENDERER_GTKRENDERER_H_ */
