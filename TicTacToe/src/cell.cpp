#include "cell.h"
#include <allegro5/allegro_primitives.h>

using namespace TicTacToe;

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

