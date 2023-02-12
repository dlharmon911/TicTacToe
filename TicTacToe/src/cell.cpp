#include "cell.h"
#include <allegro5/allegro_primitives.h>

using namespace TicTacToe;

Allegro::Bitmap Cell::m_sprites[2];

const Allegro::Color Cell::colors[Cell::COLOR_COUNT] =
{
	al_map_rgb(188,204,243),
	al_map_rgb(232,238,251),
	al_map_rgb(143,170,236),
	al_map_rgb(165,187,240),
	al_map_rgb(255, 0, 0),
	al_map_rgb(0, 0, 0),
	al_map_rgb(0, 255, 0),
	al_map_rgb(0, 0, 0)
};

Cell::Cell() : m_value(0), m_x(0), m_y(0), m_width(0), m_height(0), m_status(0) {}

Cell::Cell(const Cell& cell) : m_value(cell.m_value), m_x(cell.m_x), m_y(cell.m_y), m_width(cell.m_width), m_height(cell.m_height), m_status(cell.m_status) {}

Cell::~Cell() {}

Cell& Cell::operator = (const Cell& cell)
{
	this->m_value = cell.m_value;
	this->m_x = cell.m_x; 
	this->m_y = cell.m_y;
	this->m_width = cell.m_width;
	this->m_height = cell.m_height;
	this->m_status = cell.m_status;
	return *this;
}

bool Cell::isInsideCell(int32_t mouse_x, int32_t mouse_y)
{
	return (this->m_value == Cell::Cell_Empty && 
			mouse_x >= this->m_x &&
			mouse_x < (this->m_x + this->m_width) &&
			mouse_y >= this->m_y &&
			mouse_y < (this->m_y + this->m_height));
}

void Cell::mouseHover(int32_t mouse_x, int32_t mouse_y)
{
	this->m_status = Cell::CELL_NORMAL;

	if (this->m_value== Cell::Cell_Empty && this->isInsideCell(mouse_x, mouse_y))
	{
		this->m_status = Cell::CELL_HOVER;
	}
}

void Cell::drawCell() const
{
	const float LINE_THICKNESS = 2.0f;
	int32_t face = Cell::COLOR_FACE;

	if (this->m_status == Cell::CELL_HOVER)
	{
		face = Cell::COLOR_HOVER;
	}

	al_draw_filled_rectangle(this->m_x, this->m_y, (this->m_x + this->m_width - 1), (this->m_y + this->m_height - 1), Cell::colors[face]);
	al_draw_line(this->m_x, this->m_y, (this->m_x + this->m_width - 1), this->m_y, Cell::colors[Cell::COLOR_DARK], LINE_THICKNESS);
	al_draw_line(this->m_x, this->m_y, this->m_x, (this->m_y + this->m_height - 1), Cell::colors[Cell::COLOR_DARK], LINE_THICKNESS);
	al_draw_line(this->m_x, (this->m_y + this->m_height - 1), (this->m_x + this->m_width - 1), (this->m_y + this->m_height - 1), Cell::colors[Cell::COLOR_LIGHT], LINE_THICKNESS);
	al_draw_line((this->m_x + this->m_width - 1), this->m_y, (this->m_x + this->m_width - 1), (this->m_y + this->m_height - 1), Cell::colors[Cell::COLOR_LIGHT], LINE_THICKNESS);

	if (this->m_value != Cell::Cell_Empty)
	{
		al_draw_bitmap(Cell::m_sprites[this->m_value - 1], this->m_x, this->m_y, 0);
	}
}

bool Cell::generateSprites(int32_t cell_width, int32_t cell_height)
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
		Cell::m_sprites[i].reset(al_create_bitmap(cell_width, cell_height));
		if (!Cell::m_sprites[i].is_valid())
		{
			return false;
		}
	}

	target = al_get_target_bitmap();

	// create X
	al_set_target_bitmap(Cell::m_sprites[0]);
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
								points[triangles[t].p3].x, points[triangles[t].p3].y, Cell::colors[Cell::COLOR_X_FACE]);
	}
	
	// starting at point zero, draw lines to the next point. 
	// The first point of each subsequent line is the previous line's second point.
	int32_t lines[12] = {1, 10, 2, 3, 9, 7, 6, 11, 5, 4, 8, 0};
	int32_t p = 0;
	for (int32_t l = 0; l < 12; ++l)
	{
		al_draw_line(points[p].x, points[p].y, 
					 points[lines[l]].x, points[lines[l]].y, Cell::colors[Cell::COLOR_X_BORDER], LINE_THICKNESS);
		p = lines[l];
	}

	al_convert_mask_to_alpha(Cell::m_sprites[0], al_map_rgb(255, 0, 255));

	// create O
	al_set_target_bitmap(Cell::m_sprites[1]);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	int ow = cell_width - (cell_padding[PADDING_LEFT] + cell_padding[PADDING_RIGHT]);
	int oh = cell_height - (cell_padding[PADDING_TOP] + cell_padding[PADDING_BOTTOM]);

	al_draw_filled_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Cell::colors[Cell::COLOR_O_FACE]);
	al_draw_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Cell::colors[Cell::COLOR_O_BORDER], LINE_THICKNESS);

	ow -= (2 * O_THICKNESS_WIDTH);
	oh -= (2 * O_THICKNESS_HEIGHT);

	al_draw_filled_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, al_map_rgb(255, 0, 255));
	al_draw_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Cell::colors[Cell::COLOR_O_BORDER], LINE_THICKNESS);

	al_convert_mask_to_alpha(Cell::m_sprites[1], al_map_rgb(255, 0, 255));

	al_set_target_bitmap(target);

	return true;
}

bool Cell::freeSprites()
{
	bool rv = false;
	for (int32_t i = 0; i < 2; ++i)
	{
		if (Cell::m_sprites[i])
		{
			rv = true;
		}
		Cell::m_sprites[i].reset();
	}

	return rv;
}
