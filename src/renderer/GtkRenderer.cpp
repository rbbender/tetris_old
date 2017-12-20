/*
 * GtkRenderer.cpp
 *
 *  Created on: Dec 19, 2017
 *      Author: vgratsil
 */

#include <renderer/GtkRenderer.h>

GtkRenderer::GtkRenderer() {
}

void GtkRenderer::set_ref_game_field_area(
		Glib::RefPtr<Gtk::DrawingArea>& refDrAreaField) {
	refDrAreaGameField = refDrAreaField;
}

void GtkRenderer::set_ref_next_figure_area(
		Glib::RefPtr<Gtk::DrawingArea>& refDrAreaNextFig) {
	refDrAreaNextFigure = refDrAreaNextFig;
}

void GtkRenderer::set_ref_score_label(Glib::RefPtr<Gtk::Label>& refScoreLabel) {
	refLabelScore = refScoreLabel;
}

void GtkRenderer::set_ref_level_label(Glib::RefPtr<Gtk::Label>& refLevelLabel) {
	refLabelLevel = refLevelLabel;
}

void GtkRenderer::set_ref_cairo_target_context(
		Cairo::RefPtr<Cairo::Context>& cr) {
	refCairoContext = cr;
}

int GtkRenderer::render(double ratio) {
	if (is_full_redraw_required()) {
		refCairoContext->set_source_rgb(0.0, 0.0, 0.0);
	}
	return 0;
}

int GtkRenderer::process_input() {
	return 0;
}




