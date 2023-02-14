
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "grid.h"
#include "theme.h"

using namespace TicTacToe;

const int32_t Grid::grid_width = 3;
const int32_t Grid::grid_height = 3;

Grid::Grid() : m_cell(), m_sprites()
{
	this->m_sprites[0] = nullptr;
	this->m_sprites[1] = nullptr;
}

Grid::~Grid()
{
	for (int32_t i = 0; i < 2; ++i)
	{
		if (this->m_sprites[i])
		{
			al_destroy_bitmap(this->m_sprites[i]);
			this->m_sprites[i] = nullptr;
		}
	}
}

Grid* Grid::generateGrid(int32_t width, int32_t height)
{
	Grid* grid = new Grid();

	if (grid)
	{

		enum Padding
		{
			PADDING_TOP,
			PADDING_BOTTOM,
			PADDING_LEFT,
			PADDING_RIGHT,
			PADDING_COUNT
		};

		enum Spacing
		{
			SPACING_HORIZONTAL,
			SPACING_VERTICAL,
			SPACING_COUNT
		};

		const int32_t cell_padding[PADDING_COUNT] =
		{
			width / 80,
			width / 80,
			height / 80,
			height / 80
		};
		const int32_t cell_spacing[SPACING_COUNT] =
		{
			cell_padding[PADDING_LEFT] * 2,
			cell_padding[PADDING_TOP] * 2
		};
		const int32_t cell_width = (width - (cell_padding[PADDING_LEFT] + cell_padding[PADDING_RIGHT] + 2 * cell_spacing[SPACING_HORIZONTAL])) / grid_width;
		const int32_t cell_height = (height - (cell_padding[PADDING_TOP] + cell_padding[PADDING_BOTTOM] + 2 * cell_spacing[SPACING_VERTICAL])) / grid_height;

		for (int32_t j = 0; j < Grid::grid_height; ++j)
		{
			for (int32_t i = 0; i < Grid::grid_width; ++i)
			{
				int index = i + j * Grid::grid_width;
				int x = cell_padding[PADDING_LEFT] + i * (cell_spacing[SPACING_HORIZONTAL] + cell_width);
				int y = cell_padding[PADDING_TOP] + j * (cell_spacing[SPACING_VERTICAL] + cell_height);

				grid->m_cell[index].m_value = Cell::Cell_Empty;
				grid->m_cell[index].m_x = x;
				grid->m_cell[index].m_y = y;
				grid->m_cell[index].m_width = cell_width;
				grid->m_cell[index].m_height = cell_height;
				grid->m_cell[index].m_status = 0;
			}
		}

		grid->m_cell[0].m_value = Cell::Cell_O;
		grid->m_cell[4].m_value = Cell::Cell_X;

		if (!grid->generateSprites(cell_width, cell_height))
		{
			delete grid;
			grid = nullptr;
		}
	}

	return grid;
}

void Grid::drawGrid() const
{
	al_clear_to_color(Theme::getColor(Theme::COLOR_BACKGROUND));

	for (int32_t j = 0; j < Grid::grid_height; ++j)
	{
		for (int32_t i = 0; i < Grid::grid_width; ++i)
		{
			this->drawCell(this->m_cell[i + j * Grid::grid_width]);
		}
	}
}

int32_t Grid::isInsideCell(int32_t mouse_x, int32_t mouse_y)
{
	for (int32_t j = 0; j < Grid::grid_height; ++j)
	{
		for (int32_t i = 0; i < Grid::grid_width; ++i)
		{
			if (this->m_cell[i + j * Grid::grid_width].isInsideCell(mouse_x, mouse_y))
			{
				return (i + j * Grid::grid_width);
			}
		}
	}

	return -1;
}

void Grid::mouseHover(int32_t mouse_x, int32_t mouse_y)
{
	for (int32_t j = 0; j < Grid::grid_height; ++j)
	{
		for (int32_t i = 0; i < Grid::grid_width; ++i)
		{
			this->m_cell[i + j * Grid::grid_width].mouseHover(mouse_x, mouse_y);
		}
	}
}

int32_t Grid::evaluateGrid() const
{
	const int32_t numberOfWins = 8;
	
	typedef struct Triplet
	{
		int32_t p1;
		int32_t p2;
		int32_t p3;
	} Triplet;

	// there are 8 possible combinations of 3 cells to win
	// 3 rows, 3 columns, 2 diagonals
	Triplet triplets[numberOfWins] =
	{
		{ 0, 1, 2 },
		{ 3, 4, 5 },
		{ 6, 7, 8 },
		{ 0, 3, 6 },
		{ 1, 4, 7 },
		{ 2, 5, 8 },
		{ 0, 4, 8 },
		{ 2, 4, 6 }
	};
	int32_t winOutcomes[3] = { 0, 10, -10 };

	// go through all 8 possible combinations
	// if all 3 cells in each combination ANDed together then they match and it is a win
	for (int32_t i = 0; i < numberOfWins; ++i)
	{
		if (this->m_cell[triplets[i].p1].m_value & 
			this->m_cell[triplets[i].p2].m_value & 
			this->m_cell[triplets[i].p3].m_value) return winOutcomes[this->m_cell[triplets[i].p1].m_value];
	}

	return 0;
}

void Grid::drawCell(const Cell& cell) const
{
	const float LINE_THICKNESS = 2.0f;
	int32_t face = Theme::COLOR_FACE;

	if (cell.m_status == Cell::CELL_HOVER)
	{
		face = Theme::COLOR_HOVER;
	}

	al_draw_filled_rectangle(cell.m_x, cell.m_y, (cell.m_x + cell.m_width - 1), (cell.m_y + cell.m_height - 1), Theme::getColor(face));
	al_draw_line(cell.m_x, cell.m_y, (cell.m_x + cell.m_width - 1), cell.m_y, Theme::getColor(Theme::COLOR_DARK), LINE_THICKNESS);
	al_draw_line(cell.m_x, cell.m_y, cell.m_x, (cell.m_y + cell.m_height - 1), Theme::getColor(Theme::COLOR_DARK), LINE_THICKNESS);
	al_draw_line(cell.m_x, (cell.m_y + cell.m_height - 1), (cell.m_x + cell.m_width - 1), (cell.m_y + cell.m_height - 1), Theme::getColor(Theme::COLOR_LIGHT), LINE_THICKNESS);
	al_draw_line((cell.m_x + cell.m_width - 1), cell.m_y, (cell.m_x + cell.m_width - 1), (cell.m_y + cell.m_height - 1), Theme::getColor(Theme::COLOR_LIGHT), LINE_THICKNESS);

	if (cell.m_value != Cell::Cell_Empty)
	{
		al_draw_bitmap(Grid::m_sprites[cell.m_value - 1], cell.m_x, cell.m_y, 0);
	}
}

bool Grid::generateSprites(int32_t cell_width, int32_t cell_height)
{
	const float LINE_THICKNESS = 2.0f;
	ALLEGRO_BITMAP* target = nullptr;
	const int32_t X_CENTER = cell_width >> 1;
	const int32_t Y_CENTER = cell_height >> 1;
	const float X_THICKNESS_WIDTH = float(cell_width >> 3);
	const float X_THICKNESS_HEIGHT = float(cell_height >> 3);
	const float O_THICKNESS_WIDTH = float(cell_width >> 3) * 0.8f;
	const float O_THICKNESS_HEIGHT = float(cell_height >> 3) * 0.8f;

	enum Padding
	{
		PADDING_LEFT,
		PADDING_RIGHT,
		PADDING_TOP,
		PADDING_BOTTOM,
		PADDING_COUNT
	};

	const float cell_padding[PADDING_COUNT] =
	{
		float(cell_width >> 4),
		float(cell_width >> 4),
		float(cell_height >> 4),
		float(cell_height >> 4)
	};

	// create bitmaps
	for (int32_t i = 0; i < 2; ++i)
	{
		// create X
		this->m_sprites[i] = al_create_bitmap(cell_width, cell_height);
		if (!this->m_sprites[i])
		{
			return false;
		}
	}

	target = al_get_target_bitmap();

	// create X
	al_set_target_bitmap(Grid::m_sprites[0]);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	typedef struct Point
	{
		float x;
		float y;
	} Point;

	Point points[12] =
	{
		{cell_padding[PADDING_LEFT], cell_padding[PADDING_TOP]},
		{cell_padding[PADDING_LEFT] + X_THICKNESS_WIDTH, cell_padding[PADDING_TOP]},
		{cell_width - cell_padding[PADDING_RIGHT] - X_THICKNESS_WIDTH, cell_padding[PADDING_TOP]},
		{cell_width - cell_padding[PADDING_RIGHT], cell_padding[PADDING_TOP]},
		{cell_padding[PADDING_LEFT], cell_height - cell_padding[PADDING_BOTTOM]},
		{cell_padding[PADDING_LEFT] + X_THICKNESS_WIDTH, cell_height - cell_padding[PADDING_BOTTOM]},
		{cell_width - cell_padding[PADDING_RIGHT] - X_THICKNESS_WIDTH, cell_height - cell_padding[PADDING_BOTTOM]},
		{cell_width - cell_padding[PADDING_RIGHT], cell_height - cell_padding[PADDING_BOTTOM]},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f}
	};

	float m = (points[2].y - points[4].y) / (points[2].x - points[4].x);
	float b = (points[2].y - m * points[2].x);

	points[8].y = Y_CENTER;
	points[8].x = (points[8].y - b) / m;
	points[10].x = X_CENTER;
	points[10].y = m * points[10].x + b;

	m = (points[3].y - points[5].y) / (points[3].x - points[5].x);
	b = (points[3].y - m * points[3].x);
	points[9].y = Y_CENTER;
	points[9].x = (points[9].y - b) / m;
	points[11].x = X_CENTER;
	points[11].y = m * points[11].x + b;

	typedef struct Triangle
	{
		int32_t p1;
		int32_t p2;
		int32_t p3;
	} Triangle;

	Triangle triangles[4] =
	{
		{0, 1, 7},
		{0, 6, 7},
		{2, 3, 4},
		{3, 4, 5}
	};

	for (int32_t t = 0; t < 4; ++t)
	{
		al_draw_filled_triangle(points[triangles[t].p1].x, points[triangles[t].p1].y,
			points[triangles[t].p2].x, points[triangles[t].p2].y,
			points[triangles[t].p3].x, points[triangles[t].p3].y, Theme::getColor(Theme::COLOR_X_FACE));
	}

	// starting at point zero, draw lines to the next point. 
	// The first point of each subsequent line is the previous line's second point.
	int32_t lines[12] = { 1, 10, 2, 3, 9, 7, 6, 11, 5, 4, 8, 0 };
	int32_t p = 0;
	for (int32_t l = 0; l < 12; ++l)
	{
		al_draw_line(points[p].x, points[p].y,
			points[lines[l]].x, points[lines[l]].y, Theme::getColor(Theme::COLOR_X_BORDER), LINE_THICKNESS);
		p = lines[l];
	}

	al_convert_mask_to_alpha(Grid::m_sprites[0], al_map_rgb(255, 0, 255));

	// create O
	al_set_target_bitmap(Grid::m_sprites[1]);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	int ow = cell_width - (cell_padding[PADDING_LEFT] + cell_padding[PADDING_RIGHT]);
	int oh = cell_height - (cell_padding[PADDING_TOP] + cell_padding[PADDING_BOTTOM]);

	al_draw_filled_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Theme::getColor(Theme::COLOR_O_FACE));
	al_draw_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Theme::getColor(Theme::COLOR_O_BORDER), LINE_THICKNESS);

	ow -= (2 * O_THICKNESS_WIDTH);
	oh -= (2 * O_THICKNESS_HEIGHT);

	al_draw_filled_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, al_map_rgb(255, 0, 255));
	al_draw_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Theme::getColor(Theme::COLOR_O_BORDER), LINE_THICKNESS);

	al_convert_mask_to_alpha(Grid::m_sprites[1], al_map_rgb(255, 0, 255));

	al_set_target_bitmap(target);

	return true;
}
