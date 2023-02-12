#include "mouse.h"

using namespace TicTacToe;

Mouse::Mouse() : m_status(Mouse::Status_Idle), m_x(-1), m_y(-1) {}

Mouse::~Mouse() {}

void Mouse::OnEventAxes(int32_t x, int32_t y)
{
	this->m_x = x;
	this->m_y = y;
	this->m_status |= Mouse::Status_Moved;
}

void Mouse::OnEventButtonDown(int32_t b)
{
	if (b == 1)
	{
		this->m_status |= Mouse::Status_Left_Down;
	}
	if (b == 2)
	{
		this->m_status |= Mouse::Status_Right_Down;
	}
}

void Mouse::OnEventButtonUp(int32_t b)
{
	if (b == 1)
	{
		this->m_status &= !Mouse::Status_Left_Down;
	}
	if (b == 2)
	{
		this->m_status &= !Mouse::Status_Right_Down;
	}
}

int32_t Mouse::getX() const
{
	return this->m_x;
}

int32_t Mouse::getY() const
{
	return this->m_y;
}

int32_t Mouse::getStatus() const
{
	return this->m_status;
}

void Mouse::acknowledgeMove ()
{
	this->m_status &= (!Mouse::Status_Moved);

}

