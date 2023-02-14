#ifndef _CELL_H_
#define _CELL_H_

#include <cstdint>

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


		enum CellType
		{
			Cell_Empty,
			Cell_X,
			Cell_O
		};

		enum CellStatus
		{
			CELL_NORMAL,
			CELL_HOVER
		};

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
