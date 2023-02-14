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
			COLOR_FACE,
			COLOR_LIGHT,
			COLOR_DARK,
			COLOR_HOVER,
			COLOR_X_FACE,
			COLOR_X_BORDER,
			COLOR_O_FACE,
			COLOR_O_BORDER,
			COLOR_COUNT
		};

		static ALLEGRO_COLOR getColor(int32_t index);

	private:
		Theme();
		~Theme();

		typedef struct ColorStruct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		} ColorStruct;

		static const ColorStruct colors[Theme::COLOR_COUNT];
	};
}

#endif // !_THEME_H_
