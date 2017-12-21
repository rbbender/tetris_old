/*
 * GtkMainWindow.h
 *
 *  Created on: Dec 14, 2017
 *      Author: vgratsil
 */

#ifndef INCLUDE_GTK_GTKTETRISMAINWINDOW_H_
#define INCLUDE_GTK_GTKTETRISMAINWINDOW_H_
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <Game.h>
#include <renderer/GtkRenderer.h>

class GTKTetrisMainWindow: public Gtk::ApplicationWindow {
public:
	GTKTetrisMainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& bld);
	~GTKTetrisMainWindow();

protected:
	// Signal handlers
	// Menu new game
	void on_menu_new_game();
	// Menu quit
	void on_menu_quit();
	// Menu Help
	void on_menu_help();

	const Glib::RefPtr<Gtk::Builder>& gtkBuilder;
	Game* pGame;
	GtkRenderer renderer;
	Gtk::DrawingArea* p_gtk_game_field;
	Gtk::DrawingArea* p_gtk_next_figure;
	Gtk::Label* p_gtk_level;
	Gtk::Label* p_gtk_score;


};



#endif /* INCLUDE_GTK_GTKTETRISMAINWINDOW_H_ */
