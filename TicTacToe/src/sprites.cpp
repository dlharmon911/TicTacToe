#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <cmath>
#include "sprites.h"
#include "theme.h"

using namespace TicTacToe;

ALLEGRO_BITMAP* Sprites::m_sprites[int32_t(Sprites::SpriteList::Count)] = { nullptr, nullptr, nullptr };

Sprites::Sprites() {}

Sprites::~Sprites() {}

bool Sprites::generateSprites(int32_t cell_width, int32_t cell_height)
{
	typedef struct Point
	{
		float x;
		float y;
	} Point;

	ALLEGRO_BITMAP* target = al_get_target_bitmap();

	ALLEGRO_FONT* font = al_create_builtin_font();
	if (!font)
	{
		return false;
	}

	const char* title = "Tic Tac Toe";
	Sprites::m_sprites[int32_t(Sprites::SpriteList::Title)] = al_create_bitmap(al_get_text_width(font, title) + 4, al_get_font_line_height(font) + 4);

	if (!Sprites::m_sprites[int32_t(Sprites::SpriteList::Title)])
	{
		al_destroy_font(font);
		return false;
	}

	al_set_target_bitmap(Sprites::m_sprites[int32_t(Sprites::SpriteList::Title)]);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	for (int32_t j = -1; j <= 1; ++j)
	{
		for (int32_t i = -1; i <= 1; ++i)
		{
			al_draw_text(font, Theme::getColor(Theme::ColorList::COLOR_TITLE_BORDER), 2 + i, 2 + j, 0, title);
		}
	}

	al_draw_text(font, Theme::getColor(Theme::ColorList::COLOR_TITLE_FACE), 2, 2, 0, title);

	al_convert_mask_to_alpha(Sprites::m_sprites[int32_t(Sprites::SpriteList::Title)], al_map_rgb(255, 0, 255));

	al_destroy_font(font);


	/*******************************************************************************/

	const float LINE_THICKNESS = 2.0f;
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
		// create Human
		Sprites::m_sprites[int32_t(Sprites::SpriteList::Human) + i] = al_create_bitmap(cell_width, cell_height);
		if (!Sprites::m_sprites[int32_t(Sprites::SpriteList::Human) + i])
		{
			al_set_target_bitmap(target);
			return false;
		}
	}

	// create Human
	al_set_target_bitmap(Sprites::m_sprites[int32_t(Sprites::SpriteList::Human)]);
	al_clear_to_color(al_map_rgb(255, 0, 255));

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

	al_convert_mask_to_alpha(Sprites::m_sprites[int32_t(Sprites::SpriteList::Human)], al_map_rgb(255, 0, 255));

	/********************************************************************************
	 * create Computer
	 ********************************************************************************/
	al_set_target_bitmap(Sprites::m_sprites[int32_t(Sprites::SpriteList::Computer)]);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	int ow = cell_width - (cell_padding[PADDING_LEFT] + cell_padding[PADDING_RIGHT]);
	int oh = cell_height - (cell_padding[PADDING_TOP] + cell_padding[PADDING_BOTTOM]);

	al_draw_filled_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Theme::getColor(Theme::COLOR_O_FACE));
	al_draw_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Theme::getColor(Theme::COLOR_O_BORDER), LINE_THICKNESS);

	ow -= (2 * O_THICKNESS_WIDTH);
	oh -= (2 * O_THICKNESS_HEIGHT);

	al_draw_filled_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, al_map_rgb(255, 0, 255));
	al_draw_ellipse(X_CENTER, Y_CENTER, ow >> 1, oh >> 1, Theme::getColor(Theme::COLOR_O_BORDER), LINE_THICKNESS);

	al_convert_mask_to_alpha(Sprites::m_sprites[int32_t(Sprites::SpriteList::Computer)], al_map_rgb(255, 0, 255));

	al_set_target_bitmap(target);

	return true;
}

bool Sprites::freeSprites()
{
	bool rv = false;

	for (int32_t i = 0; i < int32_t(Sprites::SpriteList::Count); ++i)
	{
		if (Sprites::m_sprites[i])
		{
			al_destroy_bitmap(Sprites::m_sprites[i]);
			Sprites::m_sprites[i] = nullptr;
			rv = true;
		}
	}

	return rv;
}


ALLEGRO_BITMAP* Sprites::getSprite(SpriteList index)
{
	int32_t i = int32_t(index);

	if (i < 0 || i >= int32_t(Sprites::SpriteList::Count))
	{
		return nullptr;
	}
	return Sprites::m_sprites[i];
}


