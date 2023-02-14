#ifndef _GRID_H_
#define _GRID_H_

#include <allegro5/allegro.h>

namespace TicTacToe
{
	class Grid
	{
	private:
		/**
		Constructor for Grid
		@param none
		@return none
		*/
		Grid();
		Grid(const Grid& grid) = delete;
		Grid& operator = (const Grid& grid) = delete;
	public:
		/**
		Resize grid and calculate coords of cell in the grid
		@param width - width of display
		@param height - height of display
		@return Grid on success, nullptr on failure
		*/
		static Grid* generateGrid(int32_t width, int32_t height);

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

		/**
		Evaluate the value of the grid
		@param none
		@return 0 if no win or a tie, 10 on X win, -10 on O win
		*/
		static int32_t evaluateGrid(int32_t grid);

		enum class CellType
		{
			Cell_Empty,
			Cell_X,
			Cell_O
		};

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


	private:

		enum class CellStatus
		{
			CELL_NORMAL,
			CELL_HOVER
		};

		/**
		Generate the X and O sprites based on given cell width and height
		@param cell_width - width of the cell
		@param cell_height - height of the cell
		@return true on success
		*/
		bool generateSprites(int32_t cell_width, int32_t cell_height);

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
		ALLEGRO_BITMAP* m_sprites[2];
	};
}

#endif // _GRID_H_
