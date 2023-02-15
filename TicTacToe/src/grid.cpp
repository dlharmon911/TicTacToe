
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "grid.h"
#include "theme.h"
#include "sprites.h"

using namespace TicTacToe;

const int32_t Grid::grid_width = 3;
const int32_t Grid::grid_height = 3;

Grid::Grid() : m_grid(0), m_cell()
{
	for (int32_t index = 0; index < (Grid::grid_width * Grid::grid_height); ++index)
	{
		this->m_cell[index].x = 0;
		this->m_cell[index].y = 0;
		this->m_cell[index].width = 0;
		this->m_cell[index].height = 0;
		this->m_cell[index].status = Grid::CellStatus::Normal;
	}

	//testing only
	this->setCell(this->m_grid, 0, Grid::CellType::O);
	this->setCell(this->m_grid, 4, Grid::CellType::X);
}

Grid::Grid(const Grid& grid) : m_grid(grid.m_grid), m_cell()
{
	for (int32_t index = 0; index < (Grid::grid_width * Grid::grid_height); ++index)
	{
		this->m_cell[index] = grid.m_cell[index];
	}
}

Grid::~Grid() {}

Grid& Grid::operator = (const Grid& grid)
{
	this->m_grid = grid.m_grid;

	for (int32_t index = 0; index < (Grid::grid_width * Grid::grid_height); ++index)
	{
		this->m_cell[index] = grid.m_cell[index];
	}
	return *this;
}

void Grid::resizeGrid(int32_t width, int32_t height)
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

				this->m_cell[index].x = x;
				this->m_cell[index].y = y;
				this->m_cell[index].width = cell_width;
				this->m_cell[index].height = cell_height;
				this->m_cell[index].status = Grid::CellStatus::Normal;
			}
		}
}

int32_t Grid::getCellWidth() const
{
	return this->m_cell[0].width;
}

int32_t Grid::getCellHeight() const
{
	return this->m_cell[0].height;
}

void Grid::drawGrid() const
{
	const float LINE_THICKNESS = 2.0f;

	al_clear_to_color(Theme::getColor(Theme::COLOR_BACKGROUND));

	for (int32_t index = 0; index < (Grid::grid_width * Grid::grid_height); ++index)
	{
		int32_t face = Theme::COLOR_FACE;

		if (this->m_cell[index].status == Grid::CellStatus::Hover)
		{
			face = Theme::COLOR_HOVER;
		}

		al_draw_filled_rectangle(this->m_cell[index].x, this->m_cell[index].y, (this->m_cell[index].x + this->m_cell[index].width - 1), (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(face));
		al_draw_line(this->m_cell[index].x, this->m_cell[index].y, (this->m_cell[index].x + this->m_cell[index].width - 1), this->m_cell[index].y, Theme::getColor(Theme::COLOR_DARK), LINE_THICKNESS);
		al_draw_line(this->m_cell[index].x, this->m_cell[index].y, this->m_cell[index].x, (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(Theme::COLOR_DARK), LINE_THICKNESS);
		al_draw_line(this->m_cell[index].x, (this->m_cell[index].y + this->m_cell[index].height - 1), (this->m_cell[index].x + this->m_cell[index].width - 1), (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(Theme::COLOR_LIGHT), LINE_THICKNESS);
		al_draw_line((this->m_cell[index].x + this->m_cell[index].width - 1), this->m_cell[index].y, (this->m_cell[index].x + this->m_cell[index].width - 1), (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(Theme::COLOR_LIGHT), LINE_THICKNESS);

		if (this->getCell(this->m_grid, index) != Grid::CellType::Empty)
		{
			Sprites::SpriteList sp = Sprites::SpriteList(int32_t(Sprites::SpriteList::X) + int32_t(this->getCell(this->m_grid, index)) - 1);

			al_draw_bitmap(Sprites::getSprite(sp), this->m_cell[index].x, this->m_cell[index].y, 0);
		}
	}
}

int32_t Grid::isInsideCell(int32_t mouse_x, int32_t mouse_y)
{
	for (int32_t index = 0; index < (Grid::grid_width * Grid::grid_height); ++index)
	{
		if (this->getCell(this->m_grid, index) == Grid::CellType::Empty &&
			mouse_x >= this->m_cell[index].x &&
			mouse_x < (this->m_cell[index].x + this->m_cell[index].width) &&
			mouse_y >= this->m_cell[index].y &&
			mouse_y < (this->m_cell[index].y + this->m_cell[index].height))
		{
			return index;
		}
	}

	return -1;
}

void Grid::mouseHover(int32_t mouse_x, int32_t mouse_y)
{
	for (int32_t index = 0; index < (Grid::grid_width * Grid::grid_height); ++index)
	{
		this->m_cell[index].status = Grid::CellStatus::Normal;

		if (this->getCell(this->m_grid, index) == Grid::CellType::Empty &&
				mouse_x >= this->m_cell[index].x &&
				mouse_x < (this->m_cell[index].x + this->m_cell[index].width) &&
				mouse_y >= this->m_cell[index].y &&
				mouse_y < (this->m_cell[index].y + this->m_cell[index].height))
		{
			this->m_cell[index].status = Grid::CellStatus::Hover;
		}
	}
}

int32_t Grid::evaluateGrid(int32_t grid)
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
		if (int32_t(Grid::getCell(grid, triplets[i].p1)) & 
			int32_t(Grid::getCell(grid, triplets[i].p2)) &
			int32_t(Grid::getCell(grid, triplets[i].p3))) return winOutcomes[int32_t(Grid::getCell(grid, triplets[i].p1))];
	}

	return 0;
}

void Grid::setCell(int32_t& grid, int32_t cellNumber, Grid::CellType type)
{
	int32_t nullValue = 0b11;
	grid &= ~(nullValue << (2 * cellNumber));
	grid |= (int32_t(type) << (2 * cellNumber));
}

Grid::CellType Grid::getCell(int32_t grid, int32_t cellNumber)
{
	return Grid::CellType((grid >> (2 * cellNumber)) & 0b11);
}
