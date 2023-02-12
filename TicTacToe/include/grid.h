#ifndef _GRID_H_
#define _GRID_H_

#include "common.h"
#include "cell.h"

namespace TicTacToe
{
	class Grid
	{
	public:
		/**
		Constructor for Grid
		@param none
		@return none
		*/
		Grid();

		/**
		Copy constructor for Grid
		@param cell - Grid to copy
		@return none
		*/
		Grid(const Grid& grid);

		/**
		Deconstructor for Grid
		@param none
		@param none
		@return none
		*/
		~Grid();

		/**
		Assigment overload
		@param grid - Grid to copy
		@return this grid
		*/
		Grid& operator = (const Grid& grid);

		/**
		Calculates coords of cell in the grid
		@param width - width of display
		@param height - height of display
		@return 0 on success, negative value on failure
		*/
		int32_t generateGrid(int32_t width, int32_t height);

		/**
		Draw the grid to the display
		@param none
		@return none
		*/
		void drawGrid() const;

		/**
		Check to see if mouse coords are inside an empty cell
		@param mouse_x - x coord of mouse
		@param mouse_y - y coord of mouse
		@return which cell is inside or negative value if not
		*/
		int32_t isInsideCell(int32_t mouse_x, int32_t mouse_y);

		/**
		Tell grid mouse is hovering
		@param mouse_x - x coord of mouse
		@param mouse_y - y coord of mouse
		@return none
		*/
		void mouseHover(int32_t mouse_x, int32_t mouse_y);

		/**
		Evaluate the value of the grid
		@param none
		@return 0 if no win or a tie, 10 on X win, -10 on O win
		*/
		int32_t evaluateGrid() const;

		// number of cells in a row
		static const int32_t grid_width;

		// number of cells in a column
		static const int32_t grid_height;

		// background color of the grid
		static const Allegro::Color background;
	private:
		Cell m_cell[9];
	};
}

#endif // _GRID_H_
