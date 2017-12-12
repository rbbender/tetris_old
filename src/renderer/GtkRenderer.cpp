/*
 * GtkDrawingAreaRenderer.cpp
 *
 *  Created on: Dec 11, 2017
 *      Author: rbbender
 */

#include <renderer/GtkRenderer.h>

GtkRenderer::GtkRenderer(field_t* fld, const Glib::RefPtr<Gtk::Builder>& bld):
	Renderer(fld),
	gtkBuilder(bld),
	pGtkMainWnd(nullptr)
{
	gtkBuilder->get_widget("gtkMainWindow", pGtkMainWnd);
}

Gtk::ApplicationWindow* GtkRenderer::get_mainwindow() {
	return pGtkMainWnd;
}

int GtkRenderer::init() {};
int GtkRenderer::process_input() {};
int GtkRenderer::render(double rt) {};
GtkRenderer::~GtkRenderer() {};

