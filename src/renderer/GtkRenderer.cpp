/*
 * GtkRenderer.cpp
 *
 *  Created on: Dec 19, 2017
 *      Author: vgratsil
 */

#include <renderer/GtkRenderer.h>
#include <Game.h>

GtkRenderer::GtkRenderer():
	Renderer(),
	vecColorPatterns(NUM_COLORS),
	szPrevOffset(0),
	szCurOffset(0),
	pDrAreaGameField(nullptr),
	pDrAreaNextFigure(nullptr),
	pLabelLevel(nullptr),
	pLabelScore(nullptr)
{
	// filling in colors
	fill_color_patterns();
}

void GtkRenderer::fill_color_patterns() {
	// Black
	vecColorPatterns[0] = Cairo::SolidPattern::create_rgb(0.0, 0.0, 0.0);
	// Red
	vecColorPatterns[1] = Cairo::SolidPattern::create_rgb(1.0, 0.0, 0.0);
	// Orange
	vecColorPatterns[2] = Cairo::SolidPattern::create_rgb(1.0, 0.67, 0.0);
	// Yellow
	vecColorPatterns[3] = Cairo::SolidPattern::create_rgb(1.0, 1.0, 0.0);
	// Green
	vecColorPatterns[4] = Cairo::SolidPattern::create_rgb(0.0, 1.0, 0.0);
	// Blue
	vecColorPatterns[5] = Cairo::SolidPattern::create_rgb(0.0, 0.0, 1.0);
	// White
	vecColorPatterns[6] = Cairo::SolidPattern::create_rgb(1.0, 1.0, 1.0);
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
	if (ratio > 1.0)
		ratio = 1.0;
	szCurOffset = (short)(ratio * SZ_BLOCK_PX);
	//pGame->set_redraw_flag();
	if (pGame->is_redraw_required()) {
		pDrAreaGameField->queue_draw();
		pDrAreaNextFigure->queue_draw();
		std::string label("Level: ");
		label += std::to_string(pGame->get_level());
		pLabelLevel->set_label(label);
		std::string score("Score: ");
		score += std::to_string(pGame->get_score());
		pLabelScore->set_label(score);
	}
	else {
		auto pField = pGame->get_field();
		pDrAreaGameField->queue_draw_area(pField->get_prev_x() * SZ_BLOCK_PX,
				(pField->get_prev_y() - pField->get_vis_y()) * SZ_BLOCK_PX + szPrevOffset, 4 * SZ_BLOCK_PX, 4 * SZ_BLOCK_PX);
	}
	DEBUG_TRACE;
	return 0;
}

int GtkRenderer::perform_full_field_redraw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	auto pField = pGame->get_field();
	cr->set_source(vecColorPatterns[0]);
	cr->rectangle(0, 0, pDrAreaGameField->get_allocated_width(), pDrAreaGameField->get_allocated_height());
	cr->fill();
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
	auto rect_to_add = pField->get_new_rectangles();
	cr->set_source(vecColorPatterns[0]);
	for (auto i = rect_to_delete.cbegin(), e = rect_to_delete.cend(); i != e; ++i) {
		cr->rectangle(i->second * SZ_BLOCK_PX, i->first * SZ_BLOCK_PX + szPrevOffset, SZ_BLOCK_PX, SZ_BLOCK_PX);
		cr->fill();
	}
	for (auto i = rect_to_add.cbegin(), e = rect_to_add.cend(); i != e; ++i) {
		cr->set_source(vecColorPatterns[pField->get_fld_pnt(i->second, i->first)]);
		cr->rectangle(i->second * SZ_BLOCK_PX, i->first * SZ_BLOCK_PX + szCurOffset, SZ_BLOCK_PX, SZ_BLOCK_PX);
		cr->fill();
	}
	szPrevOffset = szCurOffset;
	pField->clear_deleted_rectangles();
	pField->clear_new_rectangles();
	DEBUG_TRACE;
	return 0;
}

int GtkRenderer::perform_next_figure_redraw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
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
	if (pGame->is_redraw_required())
		perform_full_field_redraw(cr);
	else
		perform_delta_field_redraw(cr);
	unset_flag_full_redraw();
	DEBUG_TRACE;
	return true;
}

bool GtkRenderer::on_next_figure_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	DEBUG_TRACE;
	perform_next_figure_redraw(cr);
	DEBUG_TRACE;
	return true;
}
