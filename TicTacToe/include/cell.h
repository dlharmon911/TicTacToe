#ifndef _CELL_H_
#define _CELL_H_

#include <cstdint>
#include "common.h"

namespace TicTacToe
{
	class Grid;

	class Cell
	{
	public:
		/**
		Constructor for Cell
		@param none
		@return none
		*/
		Cell();

		/**
		Copy constructor for Cell
		@param cell - Cell to copy
		@return none
		*/
		Cell(const Cell& cell);

		/**
		Deconstructor for Cell
		@param none
		@return none
		*/
		~Cell();

		/**
		Assigment overload
		@param cell - Cell to copy
		@return this cell
		*/
		Cell& operator = (const Cell& cell);

		/**
		Check to see if mouse coords are inside an empty cell
		@param mouse_x - x coord of mouse
		@param mouse_y - y coord of mouse
		@return true on success
		*/
		bool isInsideCell(int32_t mouse_x, int32_t mouse_y);

		/**
		Tell cell mouse is hovering
		@param mouse_x - x coord of mouse
		@param mouse_y - y coord of mouse
		@return none
		*/
		void mouseHover(int32_t mouse_x, int32_t mouse_y);

		/**
		Draw the cell to the display
		@param none
		@return none
		*/
		void drawCell() const;

		/**
		Generate the X and O sprites based on given cell width and height
		@param cell_width - width of the cell
		@param cell_height - height of the cell
		@return true on success
		*/
		static bool generateSprites(int32_t cell_width, int32_t cell_height);

		/**
		Free memory of sprites
		@param none
		@return true if any sprites were freed
		*/
		static bool freeSprites();

		enum CellType
		{
			Cell_Empty,
			Cell_X,
			Cell_O
		};

		enum Colors
		{
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

		enum CellStatus
		{
			CELL_NORMAL,
			CELL_HOVER
		};

		static const Allegro::Color colors[Cell::COLOR_COUNT];
		static Allegro::Bitmap m_sprites[2];

		friend class Grid;

	private:
		int32_t m_value;
		int32_t m_x;
		int32_t m_y;
		int32_t m_width;
		int32_t m_height;
		int32_t m_status;
	};
}

#endif _CELL_H_
