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
		Output to the screen
		@param none
		@return none
		*/
		void drawGameStatus();

		/**
		Output to the screen
		@param none
		@return none
		*/
		void draw();

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

		enum class GameState
		{
			StateUndefined,
			StateInitializing,
			StateTitleScreen,
			StateContinueScreen,
			StateShuttingdown,
			StatePlayerX,
			StatePlayerO,
			StateGameOver
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
			OptionNew,
			OptionOptions,
			OptionQuit,
			OptionContinue,
			OptionCount
		};
		TitleOptions m_option;

		static const char* optionText[int32_t(TitleOptions::OptionCount)];

		static const int32_t screenWidth;
		static const int32_t screenHeight;
		static const double logicRate;
		static const char* appTitle;
		static const char* author;
		static const int32_t gridWidth;
		static const int32_t gridHeight;
	};
}

#endif // !_APPLICATION_H_

