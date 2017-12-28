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
	// Keyboard controls
	bool on_key_press_event(GdkEventKey* key_event) override;
	// action - game over
	void on_game_over();

	const Glib::RefPtr<Gtk::Builder>& gtkBuilder;
	Game* pGame;
	GtkRenderer renderer;


};



#endif /* INCLUDE_GTK_GTKTETRISMAINWINDOW_H_ */
