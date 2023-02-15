#ifndef _THEME_H_
#define _THEME_H_

#include <allegro5/allegro.h>

namespace TicTacToe
{
	class Theme
	{
	public:
		enum ColorList
		{
			COLOR_BACKGROUND,
			COLOR_LIGHT,
			COLOR_FACE,
			COLOR_HOVER,
			COLOR_DARK,
			COLOR_X_FACE,
			COLOR_X_BORDER,
			COLOR_O_FACE,
			COLOR_O_BORDER,
			COLOR_TITLE_FACE,
			COLOR_TITLE_BORDER,
			COLOR_COUNT
		};

		static ALLEGRO_COLOR getColor(int32_t index);

		enum class ThemeOptions
		{
			ThemeLilac,
			ThemeBerry,
			ThemeEmerald,
			ThemeWedding,
			ThemeRose,
			ThemeBlue,
			ThemeTwins,
			ThemeCount
		};

		static void setTheme(ThemeOptions theme);
		static void incTheme();
		static void decTheme();

	private:
		Theme();
		~Theme();

		static ALLEGRO_COLOR makeColor(int32_t color);

		static const int32_t colors[Theme::COLOR_COUNT];
		static ThemeOptions m_theme;
	};
}

#endif // !_THEME_H_
