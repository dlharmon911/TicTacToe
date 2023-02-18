#ifndef _THEME_H_
#define _THEME_H_

#include <allegro5/allegro.h>

namespace TicTacToe
{
	class Theme
	{
	public:
		enum class Color
		{
			Background,
			Light,
			Face,
			Hover,
			Dark,
			FaceHuman,
			FaceBorder,
			ComputerFace,
			ComputerBorder,
			TitleFace,
			TitleBorder,
			Count
		};

		static ALLEGRO_COLOR getColor(Color color);

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

		static const int32_t colors[static_cast<int32_t>(Theme::Color::Count)];
		static ThemeOptions m_theme;
	};
}

#endif // !_THEME_H_
