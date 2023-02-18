#include "input.h"

using namespace TicTacToe;

Input::Input() : m_b(), m_x(-1), m_y(-1) 
{
	for (int32_t i = 0; i < int32_t(MouseButton::Count); ++i)
	{
		m_b[i] = false;
	}

	for (int32_t k = 0; k < ALLEGRO_KEY_MAX; ++k)
	{
		m_keys[k] = false;
	}
}

Input::~Input() {}

void Input::clear()
{
	this->m_x = -1;
	this->m_y = -1;

	for (int32_t i = 0; i < int32_t(MouseButton::Count); ++i)
	{
		m_b[i] = false;
	}

	for (int32_t k = 0; k < ALLEGRO_KEY_MAX; ++k)
	{
		m_keys[k] = false;
	}
}

void Input::OnEventAxes(int32_t x, int32_t y)
{
	this->m_x = x;
	this->m_y = y;
}

void Input::OnEventButtonDown(MouseButton button)
{
	this->m_b[int32_t(button)] = true;
}

void Input::OnEventButtonUp(MouseButton button)
{
	this->m_b[int32_t(button)] = false;
}

void Input::OnEventKeyDown(int32_t key)
{
	if (key >= 0 && key < ALLEGRO_KEY_MAX)
	{
		this->m_keys[key] = true;
	}
}

void Input::OnEventKeyUp(int32_t key)
{
	if (key >= 0 && key < ALLEGRO_KEY_MAX)
	{
		this->m_keys[key] = false;
	}
}

int32_t Input::getX() const
{
	return this->m_x;
}

int32_t Input::getY() const
{
	return this->m_y;
}

int32_t Input::getButton(MouseButton button) const
{
	return this->m_b[int32_t(button)];
}

int32_t Input::getKey(int32_t key) const
{
	if (key >= 0 && key < ALLEGRO_KEY_MAX)
	{
		return this->m_keys[key];
	}
	return -1;
}

