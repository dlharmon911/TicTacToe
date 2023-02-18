#include "theme.h"

using namespace TicTacToe;

int32_t palette[static_cast<int32_t>(Theme::ThemeOptions::ThemeCount)][5] =
{
	{0xdad7eb, 0x9c93c9, 0x6052a5, 0x383060, 0x100e1c}, // lilac
	{0xdcb3c2, 0xc5809a, 0xb96786, 0xad4d72, 0x961b4a}, // berry
	{0xb6c7bd, 0x85a291, 0x547c65, 0x245739, 0x0c4524}, // emerald	
	{0xeaf0f0, 0xc2d3d3, 0x85a8a8, 0x5d8b8b, 0x356f6f}, // wedding
	{0xffc6e6, 0xffb5da, 0xea7faf, 0xe14182, 0xb80142}, // rose
	{0xa6bffc, 0x4e6ff3, 0x2b33f9, 0x050a84, 0x040745}, // blue
	{0xd0b282, 0xc49f62, 0xb98c43, 0xad7823, 0xa16504} // twins
};


const int32_t Theme::colors[static_cast<int32_t>(Theme::Color::Count)] =
{
	0x000000,
	0x000000,
	0x000000,
	0x000000,
	0x000000,

	0xff0000,
	0x000000,
	0x00ff00,
	0x000000,
	0xffffff,
	0x000000
};

Theme::ThemeOptions Theme::m_theme = Theme::ThemeOptions::ThemeLilac;

Theme::Theme() {}
Theme::~Theme() {}

ALLEGRO_COLOR Theme::getColor(Color color)
{
	int32_t c = static_cast<int32_t>(color);

	if (c >= static_cast<int32_t>(Color::FaceHuman))
	{
		return Theme::makeColor(Theme::colors[c]);
	}
	
	return Theme::makeColor((palette[static_cast<int32_t>(Theme::m_theme)])[c]);
}


ALLEGRO_COLOR Theme::makeColor(int32_t color)
{
	return al_map_rgb((color & 0xff0000) >> 16, (color & 0xff00) >> 8, color & 0xff);
}

void Theme::setTheme(ThemeOptions theme)
{
	Theme::m_theme = theme;
}

void Theme::incTheme()
{
	int32_t t = static_cast<int32_t>(Theme::m_theme);
	int32_t c = static_cast<int32_t>(Theme::ThemeOptions::ThemeCount);

	++t;

	if (t == c)
	{
		t = 0;
	}

	Theme::m_theme = Theme::ThemeOptions(t);
}

void Theme::decTheme()
{
	int32_t t = static_cast<int32_t>(Theme::m_theme);
	int32_t c = static_cast<int32_t>(Theme::ThemeOptions::ThemeCount);
	
	--t;

	if (t < 0)
	{
		t = c - 1;
	}
	Theme::m_theme = Theme::ThemeOptions(t);
}

