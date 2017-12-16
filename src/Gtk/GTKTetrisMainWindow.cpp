/*
 * GTKTetrisMainWindow.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: vgratsil
 */
#include <Gtk/GTKTetrisMainWindow.h>

GTKTetrisMainWindow::GTKTetrisMainWindow(const Glib::RefPtr<Gtk::Builder>& bld):
	Gtk::ApplicationWindow(),
	gtkBuilder(bld),
	pField(nullptr)
{
	add_action("newgame", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_new_game));
	add_action("quit", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_quit));
	add_action("help", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_help));
}

GTKTetrisMainWindow::~GTKTetrisMainWindow() {};

void GTKTetrisMainWindow::on_menu_new_game() {
}

void GTKTetrisMainWindow::on_menu_quit() {
}

void GTKTetrisMainWindow::on_menu_help() {
}
