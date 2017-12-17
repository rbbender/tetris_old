/*
 * GTKTetrisMainWindow.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: vgratsil
 */
#include <Gtk/GTKTetrisMainWindow.h>
#include <iostream>

GTKTetrisMainWindow::GTKTetrisMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& bld):
	Gtk::ApplicationWindow(cobject),
	gtkBuilder(bld),
	pField(nullptr)
{
	set_title("Tetris");
	add_action("newgame", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_new_game));
	add_action("quit", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_quit));
	add_action("help", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_help));
	auto vec_actions = list_actions();
	//set_show_menubar(true);
	show_all();


}

GTKTetrisMainWindow::~GTKTetrisMainWindow() {};

void GTKTetrisMainWindow::on_menu_new_game() {
	printf("new game\n");
}

void GTKTetrisMainWindow::on_menu_quit() {
	printf("quit\n");
}

void GTKTetrisMainWindow::on_menu_help() {
	printf("help\n");
}
