#ifndef _GRID_H_
#define _GRID_H_

#include <allegro5/allegro.h>

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
		Grid(const Grid& grid);
		Grid& operator = (const Grid& grid);

		/**
		Resize grid and calculate coords of cell in the grid
		@param width - width of display
		@param height - height of display
		@return none
		*/
		void resizeGrid(int32_t width, int32_t height);

		/**
		Deconstructor for Grid
		@param none
		@param none
		@return none
		*/
		~Grid();

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

		enum class CellType
		{
			Empty,
			X,
			O
		};

		/**
		Returns the width of cell[0]
		@param none
		@return width of cell[0]
		*/
		int32_t getCellWidth() const;

		/**
		Returns the height of cell[0]
		@param none
		@return height of cell[0]
		*/
		int32_t getCellHeight() const;

	private:

		/**
		Evaluate the value of the grid
		@param none
		@return 0 if no win or a tie, 10 on X win, -10 on O win
		*/
		static int32_t evaluateGrid(int32_t grid);

		/**
		Sets the cell value
		@param cellNumber - number of the cell to set
		@param type - cell type
		@return none
		*/
		static void setCell(int32_t& grid, int32_t cellNumber, Grid::CellType type);

		/**
		Gets the cell value
		@param cellNumber - number of the cell to get
		@return cell type
		*/
		static Grid::CellType getCell(int32_t grid, int32_t cellNumber);

		enum class CellStatus
		{
			Normal,
			Hover
		};

		// number of cells in a row
		static const int32_t grid_width;

		// number of cells in a column
		static const int32_t grid_height;

		typedef struct Cell
		{
			int32_t x;
			int32_t y;
			int32_t width;
			int32_t height;
			CellStatus status;
		} Cell;

		int32_t m_grid;
		Cell m_cell[9];
	};
}

#endif // _GRID_H_
