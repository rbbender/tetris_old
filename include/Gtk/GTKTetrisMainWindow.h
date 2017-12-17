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
#include <field.h>

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
	field_t* pField;

};



#endif /* INCLUDE_GTK_GTKTETRISMAINWINDOW_H_ */
