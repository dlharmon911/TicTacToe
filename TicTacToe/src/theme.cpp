#include "theme.h"

using namespace TicTacToe;

const Theme::ColorStruct Theme::colors[Theme::COLOR_COUNT] =
{
	{ 210, 221, 247 },
	{ 188, 204, 243 },
	{ 232, 238, 251 },
	{ 143, 170, 236 },
	{ 165, 187, 240 },
	{ 255,   0,   0 },
	{   0,   0,   0 },
	{   0, 255,   0 },
	{   0,   0,   0}
};

Theme::Theme() {}
Theme::~Theme() {}

ALLEGRO_COLOR Theme::getColor(int32_t index)
{
	if (index < 0 || index >= Theme::COLOR_COUNT)
	{
		return al_map_rgb(0, 255, 0);
	}
	return al_map_rgb(Theme::colors[index].r, Theme::colors[index].g, Theme::colors[index].b);
}
