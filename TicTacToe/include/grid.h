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
		@param x - coord of grid
		@param y - coord of grid
		@param width - width of display
		@param height - height of display
		@return none
		*/
		void resizeGrid(int32_t x, int32_t y, int32_t width, int32_t height);

		/**
		Deconstructor for Grid
		@param none
		@param none
		@return none
		*/
		~Grid();

		/**
		Draw the grid to the display
		@param mouse_x - x coord of mouse
		@param mouse_y - y coord of mouse
		@return none
		*/
		void drawGrid(int32_t mouse_x, int32_t mouse_y) const;

		/**
		Check to see if mouse coords are inside an empty cell
		@param mouse_x - x coord of mouse
		@param mouse_y - y coord of mouse
		@return which cell is inside or negative value if not
		*/
		int32_t isInsideCell(int32_t mouse_x, int32_t mouse_y) const;

		enum class CellType
		{
			Empty,
			Human,
			Computer
		};

		/**
		IS the game currently playing
		@param none
		@return true if no moves have been played, otherwise false
		*/
		bool isInPlay() const;

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

		/**
		Sets the value of the cell
		@param cell - cell index
		@param type - the new type of cell
		@return none
		*/
		void setCell(int32_t cell, CellType type);

		/**
		Gets the value of the cell
		@param cell - cell index
		@return the cell type
		*/
		CellType getCell(int32_t cell) const;

		enum class GridValue
		{
			Human = 10,
			Computer = -10,
			Tie = 0,
			Undecided = -1
		};

		/**
		Determine if any moves left
		@param none
		@return true if available moves, false on no moves left
		*/
		bool isTie() const;

		/**
		Determine the value of the current grid
		@param none
		@return 10 if human won, -10 if computer won, 0 if tie, -1 if undecided and moves left
		*/
		Grid::GridValue evaluateGrid() const;

		/**
		Calculate the next move for the computer
		@param none
		@return the cell number or -1 if no moves left
		*/
		int32_t findComputerMove() const;

		/**
		Reset the grid back to starting point
		@param none
		@return none
		*/
		void reset();
	private:

		/**
		Calculate the minimum value for every possible set of moves
		@param none
		@return the minimum value
		*/
		static int32_t calculateMinimumValue(int32_t grid);

		/**
		Calculate the maximum value for every possible set of moves
		@param none
		@return the maximum value
		*/
		static int32_t calculateMaximumValue(int32_t grid);

		/**
		Evaluate the value of the grid
		@param none
		@return 0 if no win or a tie, 10 on Human win, -10 on Computer win
		*/
		static GridValue evaluateGrid(int32_t grid);

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

		// number of cells in a row
		static const int32_t gridColumnCount;

		// number of cells in a column
		static const int32_t gridRowCount;

		typedef struct Cell
		{
			int32_t x;
			int32_t y;
			int32_t width;
			int32_t height;
		} Cell;

		int32_t m_grid;
		Cell m_cell[9];
	};
}

#endif // _GRID_H_
