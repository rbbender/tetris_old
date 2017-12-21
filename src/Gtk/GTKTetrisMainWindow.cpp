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
	set_title("Tetris");
	bld->get_widget("gtkGameField", p_gtk_game_field);
	bld->get_widget("gtkNextFigDraw", p_gtk_next_figure);
	bld->get_widget("gtkLevelLabel", p_gtk_level);
	bld->get_widget("gtkScoreLabel", p_gtk_score);
	renderer.set_p_game_field_area(p_gtk_game_field);
	renderer.set_p_next_figure_area(p_gtk_next_figure);
	renderer.set_p_score_label(p_gtk_score);
	renderer.set_p_level_label(p_gtk_level);
	add_action("newgame", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_new_game));
	add_action("quit", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_quit));
	add_action("help", sigc::mem_fun(*this, &GTKTetrisMainWindow::on_menu_help));
}

GTKTetrisMainWindow::~GTKTetrisMainWindow() {};

void GTKTetrisMainWindow::on_menu_new_game() {
	printf("new game\n");
	pGame = new Game(renderer);
	p_gtk_game_field->signal_draw().connect(sigc::mem_fun(renderer, &GtkRenderer::on_game_field_draw));
	p_gtk_next_figure->signal_draw().connect(sigc::mem_fun(renderer, &GtkRenderer::on_next_figure_draw));
	pGame->set_redraw_flag();
	pGame->game_timer_cb();
}

void GTKTetrisMainWindow::on_menu_quit() {
	printf("quit\n");
}

void GTKTetrisMainWindow::on_menu_help() {
	printf("help\n");
}
