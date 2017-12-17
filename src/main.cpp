#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <controls.h>

#include <utility_func.h>

#include <gtkmm/builder.h>
#include <Gtk/GTKTetrisMainWindow.h>


int main(int argc, char* argv[]) {
	Glib::RefPtr<Gtk::Application> mainApp = Gtk::Application::create(argc,
			argv, "org.rbbender.Tetris");
	auto bld = Gtk::Builder::create_from_file("ui/mainwindow.glade");
	GTKTetrisMainWindow* mainWnd = nullptr;
	bld->get_widget_derived("gtkMainWindow", mainWnd);
	mainApp->run(*mainWnd);
    return 0;
}

