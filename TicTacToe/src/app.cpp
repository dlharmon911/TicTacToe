#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "app.h"
#include "theme.h"
#include "sprites.h"

using namespace TicTacToe;

const int32_t App::screenWidth = 800;
const int32_t App::screenHeight = 620;
const double App::logicRate = 60.0;
const char* App::appTitle = "Tic Tac Toe";
const char* App::author = "Daniel Harmon";
const int32_t App::gridWidth = 800;
const int32_t App::gridHeight = 600;
const char* App::optionText[int32_t(TitleOptions::OptionCount)] =
{
	"New Game",
	"Options",
	"Quit",
	"Continue Game"
};

int32_t App::run(const std::vector<std::string>& argList)
{
	App app;

	if (app.initialize(argList) == 0)
	{
		app.loop();
	}

	app.shutdown();

	return 0;
}

App::App() : 
	m_display(nullptr), 
	m_queue(nullptr), 
	m_timer(nullptr), 
	m_font(nullptr), 
	m_buffer(nullptr), 
	m_kill(false), 
	m_dirty(true), 
	m_counter(0), 
	m_grid(), 
	m_gameState(App::GameState::StateUndefined), 
	m_playerCountdown(0),
	m_option(App::TitleOptions::OptionNew)
{
}

App::~App()
{
}

int32_t App::initialize(const std::vector<std::string>& argList)
{
	this->m_gameState = App::GameState::StateInitializing;

	std::cout << "Initialization Begin" << std::endl;

	std::cout << "Initializing Allegro Library: ";
	if (!al_init())
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	// set the directory to the path of the exe
	ALLEGRO_PATH* basePath = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	if (basePath)
	{
		al_make_path_canonical(basePath);
		al_change_directory(al_path_cstr(basePath, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(basePath);
	}

	std::cout << "Initializing Input: ";
	if (!al_install_mouse())
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Initializing Keyboard: ";
	if (!al_install_keyboard())
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Initializing Font Addon: ";
	if (!al_init_font_addon())
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Initializing Primitives Addon: ";
	if (!al_init_primitives_addon())
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Initializing Image Addon: ";
	if (!al_init_image_addon())
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Creating Display: "; 
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	this->m_display = al_create_display(App::screenWidth, App::screenHeight);
	if (!this->m_display)
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	ALLEGRO_BITMAP* icon = al_load_bitmap("data\\images\\icon.png");
	if (icon)
	{
		al_set_display_icon(this->m_display, icon);
		al_destroy_bitmap(icon);
	}
	al_set_window_title(this->m_display, App::appTitle);

	std::cout << "Creating Buffer Bitmap: ";
	this->m_buffer = al_create_bitmap(App::screenWidth, App::screenHeight);
	if (!this->m_buffer)
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Creating Event Queue: ";
	this->m_queue = al_create_event_queue();
	if (!this->m_queue)
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Creating Logic Timer: ";
	this->m_timer = al_create_timer(1.0 / App::logicRate);
	if (!this->m_timer)
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Loading Built-in Font: ";
	this->m_font = al_create_builtin_font();
	if (!this->m_font)
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	al_register_event_source(this->m_queue, al_get_display_event_source(this->m_display));
	al_register_event_source(this->m_queue, al_get_timer_event_source(this->m_timer));
	al_register_event_source(this->m_queue, al_get_keyboard_event_source());
	al_register_event_source(this->m_queue, al_get_mouse_event_source());

	int32_t gridPosX = 0;
	int32_t gridPosY = 0;
	this->m_grid.resizeGrid(gridPosX, gridPosY, App::gridWidth, App::gridHeight);

	std::cout << "Creating Sprites: ";
	if (!Sprites::generateSprites(this->m_grid.getCellWidth(), this->m_grid.getCellHeight()))
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	srand(time(nullptr));

	al_hide_mouse_cursor(this->m_display);

	al_start_timer(this->m_timer);

	Theme::setTheme(Theme::ThemeOptions::ThemeWedding);

	this->m_gameState = App::GameState::StateTitleScreen;
	std::cout << "Initialization Complete" << std::endl << std::endl;
	
	return 0;
}

void App::shutdown()
{
	std::cout << std::endl << "Shutdown Begin" << std::endl;
	this->m_gameState = App::GameState::StateShuttingdown;

	if (this->m_display)
	{
		al_hide_mouse_cursor(this->m_display);
	}

	if (Sprites::freeSprites())
	{
		std::cout << "Sprites Destroyed" << std::endl;
	}

	if (this->m_font)
	{
		al_destroy_font(this->m_font);
		this->m_font = nullptr;
		std::cout << "Built-in Font Destroyed" << std::endl;
	}

	if (this->m_timer)
	{
		al_stop_timer(this->m_timer);
		al_destroy_timer(this->m_timer);
		this->m_timer = nullptr;
		std::cout << "Timer Destroyed" << std::endl;
	}

	if (this->m_queue)
	{
		al_destroy_event_queue(this->m_queue);
		this->m_queue = nullptr;
		std::cout << "Queue Destroyed" << std::endl;
	}

	if (this->m_buffer)
	{
		al_destroy_bitmap(this->m_buffer);
		this->m_buffer = nullptr;
		std::cout << "Buffer Bitmap Destroyed" << std::endl;
	}

	if (this->m_display)
	{
		al_destroy_display(this->m_display);
		this->m_display = nullptr;
		std::cout << "Display Destroyed" << std::endl;
	}

	std::cout << "Shutdown Complete" << std::endl << std::endl;
}

int32_t App::loop()
{
	while (!this->m_kill)
	{
		this->processInput();

		while (this->m_counter > 0)
		{
			this->doLogic();
			--this->m_counter;
		}

		if (this->m_dirty)
		{
			this->draw();
			this->m_dirty = false;
		}

		al_rest(0.01);
	}

	return 0;
}

void App::draw()
{
	static const char* prompt = "-> ";
	static const char* by = "by ";

	al_set_target_bitmap(this->m_buffer);

	al_clear_to_color(Theme::getColor(Theme::ColorList::COLOR_BACKGROUND));

	if (this->m_gameState == App::GameState::StateTitleScreen || this->m_gameState == App::GameState::StateContinueScreen)
	{
		static float w = float(App::screenWidth) / 3.0f;
		static float h = float(App::screenHeight) / 3.0f;

		static float bw = al_get_text_width(this->m_font, by);
		static float aw = al_get_text_width(this->m_font, App::author);
		static float bx = (w - (bw + aw)) / 2.0f;

		ALLEGRO_BITMAP* b = Sprites::getSprite(Sprites::SpriteList::Title);
		float dx = w * 0.1f;
		float dy = h * 0.1f;
		float dw = w * 0.8f;
		float dh = h * 0.4f;

		al_draw_scaled_bitmap(b, 0, 0, al_get_bitmap_width(b), al_get_bitmap_height(b), dx, dy, dw, dh, 0);

		float th = float(al_get_font_line_height(this->m_font)) * 1.5f;
		float tx = w / 2.0f;
		float ty = dy + dh + th * 3.0f;
		
		for (int32_t i = int32_t(App::TitleOptions::OptionNew); i <= int32_t(App::TitleOptions::OptionQuit); ++i)
		{
			const char* text = App::optionText[i];
			if (i == int32_t(App::TitleOptions::OptionNew) && this->m_gameState == App::GameState::StateContinueScreen)
			{
				text = App::optionText[int32_t(App::TitleOptions::OptionContinue)];
			}
			ALLEGRO_COLOR color = Theme::getColor(Theme::ColorList::COLOR_LIGHT);
			if (i == int32_t(this->m_option))
			{
				color = Theme::getColor(Theme::ColorList::COLOR_HOVER);
				al_draw_text(this->m_font, color, tx - al_get_text_width(this->m_font, prompt), ty + (float(i) * th), 0, prompt);
			}
			al_draw_text(this->m_font, color, tx, ty + (float(i) * th), 0, text);
		}
		al_draw_text(this->m_font, Theme::getColor(Theme::ColorList::COLOR_LIGHT), bx, h - (float(al_get_font_line_height(this->m_font)) * 1.5f), ALLEGRO_ALIGN_LEFT, by);
		al_draw_text(this->m_font, Theme::getColor(Theme::ColorList::COLOR_LIGHT), bx + bw, h - (float(al_get_font_line_height(this->m_font)) * 1.5f), ALLEGRO_ALIGN_LEFT, App::author);
		al_set_target_backbuffer(this->m_display);
		al_draw_scaled_bitmap(this->m_buffer, 0, 0, w, h, 0, 0, float(App::screenWidth), float(App::screenHeight), 0);
	}
	else
	{
		this->m_grid.drawGrid();
		this->drawGameStatus();
		al_set_target_backbuffer(this->m_display);
		al_draw_bitmap(this->m_buffer, 0, 0, 0);
	}

	al_flip_display();
}

void App::drawGameStatus()
{
	static const char* prompt = "Status: ";
	static float cw = float(al_get_text_width(this->m_font, "W"));
	static float ch = float(al_get_font_line_height(this->m_font));
	static float tx = cw / 2.0f;
	static float ty = float(this->gridHeight) + (ch / 2.0f);
	static float px = tx + float(al_get_text_width(this->m_font, prompt));
	static char output[256] = "";
	static const char* text[3] =
	{
		"Your Turn",
		"Computer Turn",
		"Game Over"
	};

	int32_t t = int32_t(this->m_gameState) - int32_t(App::GameState::StatePlayerX);

	strncpy_s(output, text[t], 256);

	al_draw_text(this->m_font, Theme::getColor(Theme::ColorList::COLOR_LIGHT), tx + 1.0f, ty + 1.0f, ALLEGRO_ALIGN_LEFT, prompt);
	al_draw_text(this->m_font, Theme::getColor(Theme::ColorList::COLOR_HOVER), tx, ty, ALLEGRO_ALIGN_LEFT, prompt);

	al_draw_text(this->m_font, Theme::getColor(Theme::ColorList::COLOR_LIGHT), px + 1.0f, ty + 1.0f, ALLEGRO_ALIGN_LEFT, output);
	al_draw_text(this->m_font, Theme::getColor(Theme::ColorList::COLOR_HOVER), px, ty, ALLEGRO_ALIGN_LEFT, output);

}

void App::doLogic()
{
	if (this->m_gameState == App::GameState::StateTitleScreen || this->m_gameState == App::GameState::StateContinueScreen)
	{

	}
	else
	{

		if (this->m_playerCountdown > 0)
		{
			--this->m_playerCountdown;
		}

		if (this->m_gameState == App::GameState::StatePlayerO && this->m_playerCountdown == 0)
		{
			int32_t cell = this->m_grid.findComputerMove();
			this->m_grid.setCell(cell, Grid::CellType::Computer);

			Grid::GridValue value = this->m_grid.evaluateGrid();

			if (value == Grid::GridValue::Undecided)
			{
				this->m_gameState = App::GameState::StatePlayerX;
			}
			else
			{
				this->m_gameState = App::GameState::StateGameOver;
			}

			this->m_dirty = true;
		}



		if (this->m_gameState == App::GameState::StatePlayerX)
		{
			int32_t insideCell = this->m_grid.isInsideCell(this->m_input.getX(), this->m_input.getY());

			if (this->m_input.getButton(Input::MouseButton::Left))
			{
			}

			if (insideCell >= 0)
			{
				if (this->m_input.getButton(Input::MouseButton::Left))
				{
					this->m_grid.setCell(insideCell, Grid::CellType::Human);

					Grid::GridValue value = this->m_grid.evaluateGrid();

					if (value == Grid::GridValue::Undecided)
					{
						this->m_gameState = App::GameState::StatePlayerO;
						this->m_playerCountdown = 60;// (60 * (rand() % 3));
					}
					else
					{
						this->m_gameState = App::GameState::StateGameOver;
					}

					this->m_dirty = true;
				}

				al_set_system_mouse_cursor(this->m_display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
			}
			else
			{
				al_set_system_mouse_cursor(this->m_display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			}
		}
	}
}

void App::processInput()
{
	static ALLEGRO_EVENT event;

	while (!al_event_queue_is_empty(this->m_queue))
	{
		al_get_next_event(this->m_queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
		{
			++this->m_counter;
		} break;

		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			this->m_dirty = true;
			this->m_input.OnEventAxes(event.mouse.x, event.mouse.y);
		} break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		{
			this->m_dirty = true;
			this->m_input.OnEventButtonDown(Input::MouseButton(event.mouse.button - 1));
		} break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			this->m_dirty = true;
			this->m_input.OnEventButtonUp(Input::MouseButton(event.mouse.button - 1));
		} break;	

		case ALLEGRO_EVENT_KEY_DOWN:
		{
			this->m_input.OnEventKeyDown(event.keyboard.keycode);
		} break;

		case ALLEGRO_EVENT_KEY_UP:
		{
			this->m_input.OnEventKeyUp(event.keyboard.keycode);

			if (event.keyboard.keycode == ALLEGRO_KEY_OPENBRACE)
			{
				Theme::incTheme();
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_CLOSEBRACE)
			{
				Theme::decTheme();
			}

			this->m_dirty = true;
			switch (this->m_gameState)
			{
			case App::GameState::StatePlayerX:
				{
					if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					{
						this->m_gameState = App::GameState::StateContinueScreen;
						this->m_option = App::TitleOptions::OptionNew;
						al_hide_mouse_cursor(this->m_display);
					}
				} break;

			case App::GameState::StateTitleScreen:
			case App::GameState::StateContinueScreen:
				{
					if (event.keyboard.keycode == ALLEGRO_KEY_ENTER ||
						event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
					{
						if (this->m_option == App::TitleOptions::OptionNew)
						{
							this->m_gameState = App::GameState::StatePlayerX;
							al_show_mouse_cursor(this->m_display);
						}
						if (this->m_option == App::TitleOptions::OptionQuit)
						{
							this->m_kill = true;
						}
					}

					if (event.keyboard.keycode == ALLEGRO_KEY_UP ||
						event.keyboard.keycode == ALLEGRO_KEY_PAD_8)
					{
						if (this->m_option == App::TitleOptions::OptionNew)
						{
							this->m_option = App::TitleOptions::OptionQuit;
						}
						else
						{
							this->m_option = App::TitleOptions(int32_t(this->m_option) - 1);
						}
					}
					if (event.keyboard.keycode == ALLEGRO_KEY_DOWN ||
						event.keyboard.keycode == ALLEGRO_KEY_PAD_2)
					{
						if (this->m_option == App::TitleOptions::OptionQuit)
						{
							this->m_option = App::TitleOptions::OptionNew;
						}
						else
						{
							this->m_option = App::TitleOptions(int32_t(this->m_option) + 1);
						}
					}
					if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					{
						this->m_kill = true;
					}
				} break;
			default: break;
			}
		} break;

		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
		{
			this->m_input.OnEventAxes(-1, -1);
			this->m_dirty = true;
		} break;
		
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
		{
			al_acknowledge_resize(event.display.source);
			this->m_dirty = true;
		} break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
		{
			this->m_kill = true;
		} break;
		}
	}
}
