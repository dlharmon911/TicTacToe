
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "grid.h"
#include "theme.h"
#include "sprites.h"

using namespace TicTacToe;

const int32_t Grid::gridColumnCount = 3;
const int32_t Grid::gridRowCount = 3;

Grid::Grid() : m_grid(0), m_cell()
{
	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		this->m_cell[index].x = 0;
		this->m_cell[index].y = 0;
		this->m_cell[index].width = 0;
		this->m_cell[index].height = 0;
	}
}

Grid::Grid(const Grid& grid) : m_grid(grid.m_grid), m_cell()
{
	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		this->m_cell[index] = grid.m_cell[index];
	}
}

Grid::~Grid() {}

Grid& Grid::operator = (const Grid& grid)
{
	this->m_grid = grid.m_grid;

	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		this->m_cell[index] = grid.m_cell[index];
	}
	return *this;
}

void Grid::resizeGrid(int32_t x, int32_t y, int32_t width, int32_t height)
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
		const int32_t cell_width = (width - (cell_padding[PADDING_LEFT] + cell_padding[PADDING_RIGHT] + 2 * cell_spacing[SPACING_HORIZONTAL])) / gridColumnCount;
		const int32_t cell_height = (height - (cell_padding[PADDING_TOP] + cell_padding[PADDING_BOTTOM] + 2 * cell_spacing[SPACING_VERTICAL])) / gridRowCount;

		for (int32_t j = 0; j < Grid::gridRowCount; ++j)
		{
			for (int32_t i = 0; i < Grid::gridColumnCount; ++i)
			{
				int index = i + j * Grid::gridColumnCount;
				int px = cell_padding[PADDING_LEFT] + i * (cell_spacing[SPACING_HORIZONTAL] + cell_width);
				int py = cell_padding[PADDING_TOP] + j * (cell_spacing[SPACING_VERTICAL] + cell_height);

				this->m_cell[index].x = x + px;
				this->m_cell[index].y = y + py;
				this->m_cell[index].width = cell_width;
				this->m_cell[index].height = cell_height;
			}
		}
}

bool Grid::isInPlay() const
{
	return (this->m_grid != 0);
}

int32_t Grid::getCellWidth() const
{
	return this->m_cell[0].width;
}

int32_t Grid::getCellHeight() const
{
	return this->m_cell[0].height;
}

void Grid::setCell(int32_t cell, CellType type)
{
	Grid::setCell(this->m_grid, cell, type);
}

Grid::CellType Grid::getCell(int32_t cell) const
{
	return Grid::getCell(this->m_grid, cell);
}

void Grid::drawGrid(int32_t mouse_x, int32_t mouse_y) const
{
	const float LINE_THICKNESS = 2.0f;
	int32_t inside = this->isInsideCell(mouse_x, mouse_y);

	al_clear_to_color(Theme::getColor(Theme::Color::Background));

	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		Theme::Color face = Theme::Color::Face;

		if (index == inside)
		{
			face = Theme::Color::Hover;
		}

		al_draw_filled_rectangle(this->m_cell[index].x, this->m_cell[index].y, (this->m_cell[index].x + this->m_cell[index].width - 1), (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(face));
		al_draw_line(this->m_cell[index].x, this->m_cell[index].y, (this->m_cell[index].x + this->m_cell[index].width - 1), this->m_cell[index].y, Theme::getColor(Theme::Color::Dark), LINE_THICKNESS);
		al_draw_line(this->m_cell[index].x, this->m_cell[index].y, this->m_cell[index].x, (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(Theme::Color::Dark), LINE_THICKNESS);
		al_draw_line(this->m_cell[index].x, (this->m_cell[index].y + this->m_cell[index].height - 1), (this->m_cell[index].x + this->m_cell[index].width - 1), (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(Theme::Color::Light), LINE_THICKNESS);
		al_draw_line((this->m_cell[index].x + this->m_cell[index].width - 1), this->m_cell[index].y, (this->m_cell[index].x + this->m_cell[index].width - 1), (this->m_cell[index].y + this->m_cell[index].height - 1), Theme::getColor(Theme::Color::Light), LINE_THICKNESS);

		if (this->getCell(this->m_grid, index) != Grid::CellType::Empty)
		{
			Sprites::SpriteList sp = Sprites::SpriteList(static_cast<int32_t>(Sprites::SpriteList::Human) + static_cast<int32_t>(this->getCell(this->m_grid, index)) - 1);

			al_draw_bitmap(Sprites::getSprite(sp), this->m_cell[index].x, this->m_cell[index].y, 0);
		}
	}
}

int32_t Grid::isInsideCell(int32_t mouse_x, int32_t mouse_y) const
{
	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		if (mouse_x >= this->m_cell[index].x &&
			mouse_x < (this->m_cell[index].x + this->m_cell[index].width) &&
			mouse_y >= this->m_cell[index].y &&
			mouse_y < (this->m_cell[index].y + this->m_cell[index].height))
		{
			return index;
		}
	}

	return -1;
}

void Grid::setCell(int32_t& grid, int32_t cellNumber, Grid::CellType type)
{
	int32_t nullValue = 0b11;
	grid &= ~(nullValue << (2 * cellNumber));
	grid |= (static_cast<int32_t>(type) << (2 * cellNumber));
}

Grid::CellType Grid::getCell(int32_t grid, int32_t cellNumber)
{
	return Grid::CellType((grid >> (2 * cellNumber)) & 0b11);
}

bool Grid::isTie() const
{
	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		if (Grid::getCell(this->m_grid, index) == Grid::CellType::Empty)
		{
			return false;
		}
	}
	return true;
}

Grid::GridValue Grid::evaluateGrid() const
{
	return Grid::evaluateGrid(this->m_grid);
}

Grid::GridValue Grid::evaluateGrid(int32_t grid)
{
	bool isTie = true;

	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		if (Grid::getCell(grid, index) == Grid::CellType::Empty)
		{
			isTie = false;
		}
	}

	if (isTie)
	{
		return GridValue::Tie;
	}

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
	GridValue winOutcomes[3] = {GridValue::Human, GridValue::Computer };

	// go through all 8 possible combinations
	// if all 3 cells in each combination ANDed together then they match and it is a win
	for (int32_t i = 0; i < numberOfWins; ++i)
	{
		if (static_cast<int32_t>(Grid::getCell(grid, triplets[i].p1)) &
			static_cast<int32_t>(Grid::getCell(grid, triplets[i].p2)) &
			static_cast<int32_t>(Grid::getCell(grid, triplets[i].p3))) return winOutcomes[static_cast<int32_t>(Grid::getCell(grid, triplets[i].p1)) - static_cast<int32_t>(CellType::Human)];
	}
	return GridValue::Undecided;
}

int32_t Grid::findComputerMove() const
{
	int32_t move = -1;
	int32_t min = 10000;
	int32_t temp = this->m_grid;

	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		if (Grid::getCell(temp, index) == Grid::CellType::Empty)
		{
			Grid::setCell(temp, index, Grid::CellType::Computer);
			int32_t value = Grid::findMaximum(temp);
			Grid::setCell(temp, index, Grid::CellType::Empty);

			if (value < min)
			{
				move = index;
				min = value;
			}
		}
	}

	return move;
}

int32_t Grid::findMinimum(int32_t grid)
{
	int32_t min = 10000;
	GridValue value = Grid::evaluateGrid(grid);

	if (value != GridValue::Undecided)
	{
		return static_cast<int32_t>(value);
	}

	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		if (Grid::getCell(grid, index) == Grid::CellType::Empty)
		{
			Grid::setCell(grid, index, Grid::CellType::Computer);
			min = std::min(min, Grid::findMaximum(grid));
			Grid::setCell(grid, index, Grid::CellType::Empty);
		}
	}

	return min;
}

int32_t Grid::findMaximum(int32_t grid)
{
	int32_t max = -10000;
	GridValue value = Grid::evaluateGrid(grid);

	if (value != GridValue::Undecided)
	{
		return static_cast<int32_t>(value);
	}

	for (int32_t index = 0; index < (Grid::gridColumnCount * Grid::gridRowCount); ++index)
	{
		if (Grid::getCell(grid, index) == Grid::CellType::Empty)
		{
			Grid::setCell(grid, index, Grid::CellType::Human);
			max = std::max(max, Grid::findMinimum(grid));
			Grid::setCell(grid, index, Grid::CellType::Empty);
		}
	}

	return max;
}

void Grid::reset()
{
	this->m_grid = 0;
}
