/*
 * GtkRenderer.h
 *
 *  Created on: Dec 19, 2017
 *      Author: vgratsil
 */

#ifndef INCLUDE_RENDERER_GTKRENDERER_H_
#define INCLUDE_RENDERER_GTKRENDERER_H_

#include <renderer/Renderer.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/dialog.h>
#include <gtkmm/button.h>
#include <colors.h>
#include <vector>
#include <utility_func.h>

class GTKTetrisMainWindow;

class GtkRenderer: public Renderer {
	std::vector<Cairo::RefPtr<Cairo::SolidPattern>> vecColorPatterns;
	unsigned short szPrevOffset, szCurOffset;
	GTKTetrisMainWindow* pTetrisMainWnd;
	Gtk::DrawingArea* pDrAreaGameField;
	Gtk::DrawingArea* pDrAreaNextFigure;
	Gtk::Label* pLabelScore;
	Gtk::Label* pLabelLevel;

	Gtk::Dialog* pGameResultsDialog;
	Gtk::Label* pGameResultsLabel;
	Gtk::Button* pGameResultsOkButton;

	const unsigned SZ_BLOCK_PX = 20;
	std::vector<std::vector<FieldAddr_t>> blocks_colors;

	void fill_color_patterns();
	void rebuild_blocks_colors();
	void draw_blocks_colors(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_current_figure(const Cairo::RefPtr<Cairo::Context>& cr);

public:
	GtkRenderer();
	void set_widgets_from_builder(const Glib::RefPtr<Gtk::Builder>& builder);
	void connect_callbacks();
	void set_p_game_field_area(Gtk::DrawingArea* pDrAreaField);
	void set_p_next_figure_area(Gtk::DrawingArea* pDrAreaNextFig);
	void set_p_score_label(Gtk::Label* pScoreLabel);
	void set_p_level_label(Gtk::Label* pLevelLabel);

	int render(double ratio) override;
	int process_input() override;
	int perform_full_field_redraw(const Cairo::RefPtr<Cairo::Context>& cr);
	int perform_delta_field_redraw(const Cairo::RefPtr<Cairo::Context>& cr);
	int perform_next_figure_redraw(const Cairo::RefPtr<Cairo::Context>& cr);

	bool on_game_field_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_next_figure_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	int wrap_up();
	void show_game_stats();
	void on_game_stats_ok(int response);
	void clean_up() override;


};

#endif /* INCLUDE_RENDERER_GTKRENDERER_H_ */
