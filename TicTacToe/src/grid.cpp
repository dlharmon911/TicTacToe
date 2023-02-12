
#include <iostream>
#include "grid.h"
#include <allegro5/allegro_primitives.h>

using namespace TicTacToe;

const int32_t Grid::grid_width = 3;
const int32_t Grid::grid_height = 3;
const Allegro::Color Grid::background = al_map_rgb(210, 221, 247);

Grid::Grid() : m_cell() {}

Grid::Grid(const Grid& grid) : m_cell()
{
	for (int32_t j = 0; j < Grid::grid_height; ++j)
	{
		for (int32_t i = 0; i < Grid::grid_width; ++i)
		{
			this->m_cell[i + j * Grid::grid_width] = grid.m_cell[i + j * Grid::grid_width];
		}
	}
}

Grid::~Grid() {}

Grid& Grid::operator = (const Grid& grid)
{
	for (int32_t i = 0; i < 9; ++i)
	{
		this->m_cell[i] = grid.m_cell[i];
	}

	return *this;
}

int32_t Grid::generateGrid(int32_t width, int32_t height)
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

			this->m_cell[index].m_value = Cell::Cell_Empty;
			this->m_cell[index].m_x = x;
			this->m_cell[index].m_y = y;
			this->m_cell[index].m_width = cell_width;
			this->m_cell[index].m_height = cell_height;
			this->m_cell[index].m_status = 0;
		}
	}

	this->m_cell[0].m_value = Cell::Cell_O;
	this->m_cell[4].m_value = Cell::Cell_X;

	std::cout << "Creating X/O Sprites: ";
	if (!Cell::generateSprites(cell_width, cell_height))
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;
	
	return 0;
}

void Grid::drawGrid() const
{
	al_clear_to_color(Grid::background);

	for (int32_t j = 0; j < Grid::grid_height; ++j)
	{
		for (int32_t i = 0; i < Grid::grid_width; ++i)
		{
			this->m_cell[i + j * Grid::grid_width].drawCell();
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
