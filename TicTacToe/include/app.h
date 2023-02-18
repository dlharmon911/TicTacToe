#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <string>
#include "grid.h"
#include "input.h"

namespace TicTacToe
{
	class App
	{
	public:

		/**
		Runs the program.
		@param arglist - command line arguments
		@return 0 with no errors, negative on error
		*/
		static int32_t run(const std::vector<std::string>& argList);

	private:

		/**
		App class constructor
		@param none
		@return none
		*/
		App();

		/**
		App class deconstructor
		@param none
		@return none
		*/
		~App();

		/**
		Initializes applaction
		@param arglist - command line arguments
		@return 0 with no errors, negative on error
		*/
		int32_t initialize(const std::vector<std::string>& argList);

		/**
		Shutdown and deinitialze everything
		@param none
		@return none
		*/
		void shutdown();

		/**
		Basic loop of input/logic/draw
		@param none
		@return 0 with no errors, negative on error
		*/
		int32_t loop();

		/**
		Output to the display
		@param none
		@return none
		*/
		void draw() const;

		/**
		Output to the screen
		@param none
		@return none
		*/
		void drawStatus() const;

		/**
		Output game grid to the display
		@param none
		@return none
		*/
		void drawGame() const;

		/**
		Output title screen to the display
		@param none
		@return none
		*/
		void drawTitle() const;

		/**
		Output help screen to the display
		@param none
		@return none
		*/
		void drawAbout() const;

		/**
		Output help screen to the display
		@param none
		@return none
		*/
		void drawHelp() const;

		/**
		Process all logic
		@param none
		@return none
		*/
		void doLogic();

		/**
		Process any incoming events
		@param none
		@return none
		*/
		void processInput();

		/**
		Draws a framed text box with given text
		@param text - vector of strings
		@return none
		*/
		void drawTextBox(const std::vector<std::string>& text) const;

		enum class GameState
		{
			Undefined,
			Initializing,
			TitleScreen,
			HelpScreen,
			AboutScreen,
			Shuttingdown,
			PlayerXTurn,
			PlayerOTurn,
			PlayerXWin,
			PlayerOWin,
			Tie
		};

		ALLEGRO_DISPLAY* m_display;
		ALLEGRO_EVENT_QUEUE* m_queue;
		ALLEGRO_TIMER* m_timer;
		ALLEGRO_FONT* m_font;
		ALLEGRO_BITMAP* m_buffer;
		bool m_kill;
		bool m_dirty;
		int32_t m_counter;
		Grid m_grid;
		Input m_input;
		GameState m_gameState;
		int32_t m_playerCountdown;

		enum class TitleOptions
		{
			New,
			Help,
			About,
			Quit,
			Continue,
			Count
		};
		TitleOptions m_option;

		static const std::string optionText[static_cast<int32_t>(TitleOptions::Count)];

		static const int32_t screenWidth;
		static const int32_t screenHeight;
		static const double logicRate;
		static const std::string appTitle;
		static const std::string author;
		static const int32_t gridWidth;
		static const int32_t gridHeight;
	};
}

#endif // !_APPLICATION_H_

