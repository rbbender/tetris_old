/*
 * GtkRenderer.cpp
 *
 *  Created on: Dec 19, 2017
 *      Author: vgratsil
 */

#include <renderer/GtkRenderer.h>
#include <Gtk/GTKTetrisMainWindow.h>
#include <Game.h>

GtkRenderer::GtkRenderer():
	Renderer(),
	vecColorPatterns(NUM_COLORS),
	szPrevOffset(0),
	szCurOffset(0),
	pDrAreaGameField(nullptr),
	pDrAreaNextFigure(nullptr),
	pLabelScore(nullptr),
	pLabelLevel(nullptr),
	pGameResultsDialog(nullptr),
	pGameResultsLabel(nullptr),
	pGameResultsOkButton(nullptr),
	blocks_colors(NUM_COLORS)
{
	// filling in colors
	fill_color_patterns();
}

void GtkRenderer::fill_color_patterns() {
	// Black
	vecColorPatterns[BLACK] = Cairo::SolidPattern::create_rgb(0.0, 0.0, 0.0);
	// Red
	vecColorPatterns[RED] = Cairo::SolidPattern::create_rgb(1.0, 0.0, 0.0);
	// Orange
	vecColorPatterns[ORANGE] = Cairo::SolidPattern::create_rgb(1.0, 0.67, 0.0);
	// Yellow
	vecColorPatterns[YELLOW] = Cairo::SolidPattern::create_rgb(1.0, 1.0, 0.0);
	// Green
	vecColorPatterns[GREEN] = Cairo::SolidPattern::create_rgb(0.0, 1.0, 0.0);
	// Blue
	vecColorPatterns[BLUE] = Cairo::SolidPattern::create_rgb(0.0, 0.0, 1.0);
	// White
	vecColorPatterns[WHITE] = Cairo::SolidPattern::create_rgb(1.0, 1.0, 1.0);
}

void GtkRenderer::set_p_game_field_area(
		Gtk::DrawingArea* pDrAreaField) {
	pDrAreaGameField = pDrAreaField;
}

void GtkRenderer::set_p_next_figure_area(
		Gtk::DrawingArea* pDrAreaNextFig) {
	pDrAreaNextFigure = pDrAreaNextFig;
}

void GtkRenderer::set_p_score_label(Gtk::Label* pScoreLabel) {
	pLabelScore = pScoreLabel;
}

void GtkRenderer::set_p_level_label(Gtk::Label* pLevelLabel) {
	pLabelLevel = pLevelLabel;
}



int GtkRenderer::render(double ratio) {
	DEBUG_TRACE;
	//set_flag_full_redraw();
	if (ratio >= 1.0)
		ratio = 0.0;
	szCurOffset = (short)(ratio * SZ_BLOCK_PX);
	DEBUG_VAR("%u\n", szCurOffset);
	if (pGame) {
		if (pGame->is_redraw_required()) {
			rebuild_blocks_colors();
			pDrAreaNextFigure->queue_draw();
			std::string label("Level: ");
			label += std::to_string(pGame->get_level());
			pLabelLevel->set_label(label);
			std::string score("Score: ");
			score += std::to_string(pGame->get_score());
			pLabelScore->set_label(score);
		}
		pDrAreaGameField->queue_draw();
	}
	else {
		pDrAreaGameField->queue_draw();
		pDrAreaNextFigure->queue_draw();
		pLabelLevel->set_label("Level: 1");
		pLabelLevel->set_label("Score: 0");
		show_game_stats();
	}
	DEBUG_TRACE;
	return 0;
}

int GtkRenderer::perform_full_field_redraw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	cr->set_source(vecColorPatterns[0]);
	cr->rectangle(0, 0, pDrAreaGameField->get_allocated_width(), pDrAreaGameField->get_allocated_height());
	cr->fill();
	if (!pGame)
		return 0;
	auto pField = pGame->get_field();
	for (int k = pField->get_vis_y(); k < pField->get_field_size_y(); ++k)
		for (int i = 0; i < pField->get_field_size_x(); ++i) {
			if (pField->get_fld_pnt(i, k) != BLACK) {
				cr->set_source(vecColorPatterns[pField->get_fld_pnt(i, k)]);
				cr->rectangle(i * SZ_BLOCK_PX, (k - pField->get_vis_y()) * SZ_BLOCK_PX, SZ_BLOCK_PX, SZ_BLOCK_PX);
				cr->fill();
			}
		}
	pGame->unset_redraw_flag();
	DEBUG_TRACE;
	return 0;
}

int GtkRenderer::perform_delta_field_redraw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	auto pField = pGame->get_field();
	auto rect_to_delete = pField->get_deleted_rectangles();
	std::vector<Cairo::RectangleInt> rect_int_to_delete;


	szPrevOffset = szCurOffset;
	DEBUG_TRACE;
	return 0;
}

int GtkRenderer::perform_next_figure_redraw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	cr->set_source(vecColorPatterns[0]);
	cr->rectangle(0, 0, pDrAreaNextFigure->get_allocated_width(), pDrAreaNextFigure->get_allocated_height());
	cr->fill();
	if (!pGame)
		return 0;
	auto next_pos = pGame->get_next_position();
	auto next_col = pGame->get_next_color();
	cr->set_source(vecColorPatterns[next_col]);
	for (auto k = 0; k < next_pos->size_y; ++k)
		for (auto i = 0; i < next_pos->size_x; ++i) {
			if (next_pos->layout[k][i] == 1) {
				cr->rectangle(i * SZ_BLOCK_PX, k * SZ_BLOCK_PX, SZ_BLOCK_PX, SZ_BLOCK_PX);
				cr->fill();
			}
		}
	DEBUG_TRACE;
	return 0;
}

int GtkRenderer::process_input() {
	return 0;
}

bool GtkRenderer::on_game_field_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	if (!pGame) {
		draw_blocks_colors(cr);
		pLabelScore->set_label("Score: 0");
		pLabelLevel->set_label("Level: 1");
		return true;
	}
	draw_blocks_colors(cr);
	draw_current_figure(cr);
	pGame->unset_redraw_flag();
	DEBUG_TRACE;
	return true;
}

void GtkRenderer::draw_blocks_colors(const Cairo::RefPtr<Cairo::Context>& cr) {
	cr->set_source(vecColorPatterns[BLACK]);
	cr->paint();
	for (int i = 0; i < NUM_COLORS; ++i) {
		if (i != BLACK) {
			for (auto p = blocks_colors[i].cbegin(), pe = blocks_colors[i].cend(); p != pe; ++p){
				cr->rectangle(SZ_BLOCK_PX * (p->second), SZ_BLOCK_PX * p->first, SZ_BLOCK_PX, SZ_BLOCK_PX);
			}
			cr->set_source(vecColorPatterns[i]);
			cr->fill();
		}
	}
}

bool GtkRenderer::on_next_figure_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	perform_next_figure_redraw(cr);
	DEBUG_TRACE;
	if (!pGame)
		return false;
	return true;
}

void GtkRenderer::rebuild_blocks_colors() {
	// clearing previous info
	for (auto i = blocks_colors.begin(), e = blocks_colors.end(); i != e; ++i)
		i->clear();
	if (!pGame)
		return;
	// filling blocks_colors
	auto pField = pGame->get_field();
	pField->remove_previous();
	auto ke = pField->get_field_size_y();
	auto ie = pField->get_field_size_x();
	auto visy = pField->get_vis_y();
	for (int k = visy; k < ke; ++k)
		for (int i = 0; i < ie; ++i)
			if (pField->get_fld_pnt(i, k) != BLACK)
				blocks_colors[pField->get_fld_pnt(i, k)].emplace_back(k - visy, i);
	pField->recompose();
}

void GtkRenderer::draw_current_figure(const Cairo::RefPtr<Cairo::Context>& cr) {
	if (!pGame)
		return;
	auto p_cur_fig = pGame->get_field()->get_current_figure_p();
	auto visy = pGame->get_field()->get_vis_y();
	for (int k = 0; k < p_cur_fig->current_pos->size_y; ++k)
		for (int i = 0; i < p_cur_fig->current_pos->size_x; ++i)
			if (p_cur_fig->current_pos->layout[k][i] == 1) {
				int pix_x = SZ_BLOCK_PX * (p_cur_fig->pos_x + i);
				int pix_y = SZ_BLOCK_PX * (p_cur_fig->pos_y + k - visy) + szCurOffset;
				DEBUG_VAR("%d\n", pix_x);
				DEBUG_VAR("%d\n", pix_y);
				cr->rectangle(pix_x, pix_y,	SZ_BLOCK_PX, SZ_BLOCK_PX);
			}
	cr->set_source(vecColorPatterns[p_cur_fig->color]);
	cr->fill();
}

int GtkRenderer::wrap_up() {
	for (auto i = blocks_colors.begin(), ie = blocks_colors.end(); i != ie; ++i)
		i->clear();
	std::string strFinalResult("Game is over. Final score is: ");
	strFinalResult += std::to_string(pGame->get_score());
	pGameResultsLabel->set_label(strFinalResult);
	pGame = nullptr;
	return 0;
}

void GtkRenderer::show_game_stats() {
	DEBUG_TRACE;
	int result = pGameResultsDialog->run();
	DEBUG_VAR("%d\n", result);
	pGameResultsDialog->hide();
	DEBUG_TRACE;
}

void GtkRenderer::set_widgets_from_builder(
		const Glib::RefPtr<Gtk::Builder>& builder) {
	builder->get_widget_derived("gtkMainWindow", pTetrisMainWnd);
	builder->get_widget("gtkGameField", pDrAreaGameField);
	builder->get_widget("gtkNextFigDraw", pDrAreaNextFigure);
	builder->get_widget("gtkLevelLabel", pLabelLevel);
	builder->get_widget("gtkScoreLabel", pLabelScore);
	builder->get_widget("gtkGameResultsWindow", pGameResultsDialog);
	builder->get_widget("gtkGameResultsWindow_ResultsLabel", pGameResultsLabel);
	builder->get_widget("gtkGameResultsWindow_OkButton", pGameResultsOkButton);
	pGameResultsDialog->signal_response().connect(sigc::mem_fun(*this, &GtkRenderer::on_game_stats_ok));
}

void GtkRenderer::connect_callbacks() {
	pDrAreaGameField->signal_draw().connect(sigc::mem_fun(*this, &GtkRenderer::on_game_field_draw));
	pDrAreaNextFigure->signal_draw().connect(sigc::mem_fun(*this, &GtkRenderer::on_next_figure_draw));
}

void GtkRenderer::on_game_stats_ok(int response) {
	DEBUG_VAR("%d\n", response);
}

void GtkRenderer::clean_up() {
	pTetrisMainWnd->destroy_game();
}
