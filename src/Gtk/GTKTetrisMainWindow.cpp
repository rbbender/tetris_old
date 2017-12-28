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
	pGame(nullptr),
	renderer()
{
	renderer.set_widgets_from_builder(bld);
	set_title("Tetris");
	add_action("newgame", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_new_game));
	add_action("cancelgame", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_game_over));
	add_action("quit", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_quit));
	add_action("help", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_help));
	add_events(Gdk::KEY_PRESS_MASK);
}

GTKTetrisMainWindow::~GTKTetrisMainWindow() {};

void GTKTetrisMainWindow::on_menu_new_game() {
	printf("new game\n");
	pGame = new Game(renderer);
	pGame->set_redraw_flag();
	pGame->game_timer_cb();
}

void GTKTetrisMainWindow::on_menu_quit() {
	printf("quit\n");
}

void GTKTetrisMainWindow::on_menu_help() {
	printf("help\n");
}

bool GTKTetrisMainWindow::on_key_press_event(GdkEventKey* key_event) {
	switch (key_event->keyval) {
	case GDK_KEY_H:
	case GDK_KEY_h:
		pGame->process_move_left();
		break;
	case GDK_KEY_J:
	case GDK_KEY_j:
		pGame->process_rotate_clockwise();
		break;
	case GDK_KEY_K:
	case GDK_KEY_k:
		pGame->process_rotate_counterclockwise();
		break;
	case GDK_KEY_L:
	case GDK_KEY_l:
		pGame->process_move_right();
		break;
	case GDK_KEY_space:
		pGame->process_force_landing();
		break;
	case GDK_KEY_Q:
	case GDK_KEY_q:
		pGame->process_quit();
		break;
	}
	return true;
}

void GTKTetrisMainWindow::on_game_over() {
	renderer.wrap_up();
	delete pGame;
	pGame = nullptr;
}
