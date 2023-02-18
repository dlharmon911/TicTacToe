#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <allegro5/allegro5.h>

namespace TicTacToe
{
	class Sprites
	{
	public:

		/**
		Generate the Human and Computer sprites based on given cell width and height
		@param cell_width - width of the cell
		@param cell_height - height of the cell
		@return true on success
		*/
		static bool generateSprites(int32_t cell_width, int32_t cell_height);
		
		/**
		Release memory of sprites
		@param none
		@return true if any were freed, false if none were freed
		*/		
		static bool freeSprites();

		enum class SpriteList
		{
			Title,
			Human,
			Computer,
			Count
		};

		static ALLEGRO_BITMAP* getSprite(SpriteList index);

	private:
		Sprites();
		~Sprites();

		static ALLEGRO_BITMAP* m_sprites[static_cast<int32_t>(Sprites::SpriteList::Count)];
	};
}

#endif // !_SPRITES_H_
