#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "app.h"
#include "theme.h"
#include "sprites.h"

using namespace TicTacToe;

const int32_t App::SCREEN_WIDTH = 800;
const int32_t App::SCREEN_HEIGHT = 600;
const double App::LOGIC_RATE = 60.0;
const char* App::APP_NAME = "Tic Tac Toe";
const char* App::option_text[int32_t(TitleOptions::Count)] =
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

App::App() : m_display(nullptr), m_queue(nullptr), m_timer(nullptr), m_font(nullptr), m_kill(false), m_dirty(true), m_counter(0), m_grid(), m_gameState(App::GameState::StateUndefined), m_option(App::TitleOptions::New)
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

	std::cout << "Initializing Mouse: ";
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
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	this->m_display = al_create_display(App::SCREEN_WIDTH, App::SCREEN_HEIGHT);
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
	al_set_window_title(this->m_display, App::APP_NAME);

	std::cout << "Creating Event Queue: ";
	this->m_queue = al_create_event_queue();
	if (!this->m_queue)
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	std::cout << "Creating Logic Timer: ";
	this->m_timer = al_create_timer(1.0 / App::LOGIC_RATE);
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

	this->m_grid.resizeGrid(App::SCREEN_WIDTH, App::SCREEN_HEIGHT);

	std::cout << "Creating Sprites: ";
	if (!Sprites::generateSprites(this->m_grid.getCellWidth(), this->m_grid.getCellHeight()))
	{
		std::cout << "failed" << std::endl;
		return -1;
	}
	std::cout << "pass" << std::endl;

	srand(time(nullptr));

	al_start_timer(this->m_timer);

	Theme::setTheme(Theme::ThemeOptions::ThemeWedding);

	std::cout << "Initialization Complete" << std::endl << std::endl;
	
	this->m_gameState = App::GameState::StateTitleScreen;
	
	return 0;
}

void App::shutdown()
{
	this->m_gameState = App::GameState::StateShuttingdown;
	std::cout << std::endl << "Shutdown Begin" << std::endl;

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
	al_clear_to_color(Theme::getColor(Theme::ColorList::COLOR_BACKGROUND));

	if (this->m_gameState == App::GameState::StateTitleScreen || this->m_gameState == App::GameState::StateContinueScreen)
	{
		ALLEGRO_BITMAP* b = Sprites::getSprite(Sprites::SpriteList::Title);
		float dx = float(App::SCREEN_WIDTH) * 0.1f;
		float dy = float(App::SCREEN_HEIGHT) * 0.1f;
		float dw = float(App::SCREEN_WIDTH) * 0.8f;
		float dh = float(App::SCREEN_HEIGHT) * 0.4f;

		al_draw_scaled_bitmap(b, 0, 0, al_get_bitmap_width(b), al_get_bitmap_height(b), dx, dy, dw, dh, 0);

		float th = (al_get_font_line_height(this->m_font)) + 4.0f;
		float tx = (float(App::SCREEN_WIDTH) - al_get_text_width(this->m_font, App::option_text[int32_t(App::TitleOptions::Continue)])) / 2.0f;
		float ty = dy + dh + th * 4.0f;
		

		for (int32_t i = int32_t(App::TitleOptions::New); i <= int32_t(App::TitleOptions::Quit); ++i)
		{
			const char* text = App::option_text[i];
			if (i == int32_t(App::TitleOptions::New) && this->m_gameState == App::GameState::StateContinueScreen)
			{
				text = App::option_text[int32_t(App::TitleOptions::Continue)];
			}
			ALLEGRO_COLOR color = Theme::getColor(Theme::ColorList::COLOR_LIGHT);
			if (i == int32_t(this->m_option))
			{
				color = Theme::getColor(Theme::ColorList::COLOR_HOVER);
			}
			al_draw_text(this->m_font, color, tx, ty + (float(i) * th), 0, text);
		}
	}
	else
	{
		this->m_grid.drawGrid();
	}

	al_flip_display();
}

void App::doLogic()
{
	if (this->m_gameState == App::GameState::StateTitleScreen || this->m_gameState == App::GameState::StateContinueScreen)
	{
	}

	if (this->m_gameState == App::GameState::StatePlayerX)
	{
		if (this->m_mouse.getStatus() & Mouse::Status_Moved)
		{
			this->m_grid.mouseHover(this->m_mouse.getX(), this->m_mouse.getY());
			this->m_mouse.acknowledgeMove();
		}

		if (this->m_grid.isInsideCell(this->m_mouse.getX(), this->m_mouse.getY()) >= 0)
		{
			al_set_system_mouse_cursor(this->m_display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
		}
		else
		{
			al_set_system_mouse_cursor(this->m_display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
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
			if (this->m_gameState == App::GameState::StatePlayerX)
			{
				this->m_mouse.OnEventAxes(event.mouse.x, event.mouse.y);
			}
		} break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		{
			this->m_dirty = true;
			if (this->m_gameState == App::GameState::StatePlayerX)
			{
				this->m_mouse.OnEventButtonDown(event.mouse.button);
			}
		} break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			this->m_dirty = true;
			if (this->m_gameState == App::GameState::StatePlayerX)
			{
				this->m_mouse.OnEventButtonUp(event.mouse.button);
			}
		} break;		

		case ALLEGRO_EVENT_KEY_UP:
		{
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
						this->m_option = App::TitleOptions::New;
					}
				} break;

			case App::GameState::StateTitleScreen:
			case App::GameState::StateContinueScreen:
				{
					if (event.keyboard.keycode == ALLEGRO_KEY_ENTER ||
						event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
					{
						if (this->m_option == App::TitleOptions::New)
						{
							this->m_gameState = App::GameState::StatePlayerX;
						}
						if (this->m_option == App::TitleOptions::Quit)
						{
							this->m_kill = true;
						}
					}

					if (event.keyboard.keycode == ALLEGRO_KEY_UP ||
						event.keyboard.keycode == ALLEGRO_KEY_PAD_8)
					{
						if (this->m_option == App::TitleOptions::New)
						{
							this->m_option = App::TitleOptions::Quit;
						}
						else
						{
							this->m_option = App::TitleOptions(int32_t(this->m_option) - 1);
						}
					}
					if (event.keyboard.keycode == ALLEGRO_KEY_DOWN ||
						event.keyboard.keycode == ALLEGRO_KEY_PAD_2)
					{
						if (this->m_option == App::TitleOptions::Quit)
						{
							this->m_option = App::TitleOptions::New;
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
			this->m_mouse.OnEventAxes(-1, -1);
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

