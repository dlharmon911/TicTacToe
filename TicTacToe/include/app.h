#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <allegro5/allegro.h>
#include <vector>
#include <string>
#include "common.h"
#include "grid.h"
#include "mouse.h"

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

		Allegro::Display m_display;
		Allegro::EventQueue m_queue;
		Allegro::Timer m_timer;
		bool m_kill;
		bool m_dirty;
		int32_t m_counter;
		Grid m_grid;
		Mouse m_mouse;

		static const int32_t SCREEN_WIDTH;
		static const int32_t SCREEN_HEIGHT;
		static const double LOGIC_RATE;
		static const char* APP_NAME;
	};
}

#endif // !_APPLICATION_H_

