/*
 * GtkRenderer.h
 *
 *  Created on: Dec 19, 2017
 *      Author: vgratsil
 */

#ifndef INCLUDE_RENDERER_GTKRENDERER_H_
#define INCLUDE_RENDERER_GTKRENDERER_H_

#include <renderer/Renderer.h>
#include <map>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>

class GtkRenderer: public Renderer {
	Glib::RefPtr<Gtk::DrawingArea> refDrAreaGameField;
	Glib::RefPtr<Gtk::DrawingArea> refDrAreaNextFigure;
	Glib::RefPtr<Gtk::Label> refLabelScore;
	Glib::RefPtr<Gtk::Label> refLabelLevel;
	Cairo::RefPtr<Cairo::Context> refCairoContext;


public:
	GtkRenderer();
	void set_ref_game_field_area(Glib::RefPtr<Gtk::DrawingArea>& refDrAreaField);
	void set_ref_next_figure_area(Glib::RefPtr<Gtk::DrawingArea>& refDrAreaNextFig);
	void set_ref_score_label(Glib::RefPtr<Gtk::Label>& refScoreLabel);
	void set_ref_level_label(Glib::RefPtr<Gtk::Label>& refLevelLabel);
	void set_ref_cairo_target_context(Cairo::RefPtr<Cairo::Context>& cr);

	int render(double ratio) override;
	int process_input() override;
};

#endif /* INCLUDE_RENDERER_GTKRENDERER_H_ */
