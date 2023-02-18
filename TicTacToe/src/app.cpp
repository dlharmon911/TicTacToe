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
const std::string App::appTitle = "Tic Tac Toe";
const std::string App::author = "Daniel Harmon";
const int32_t App::gridWidth = 800;
const int32_t App::gridHeight = 600;
const std::string App::optionText[static_cast<int32_t>(TitleOptions::Count)] =
{
	"New Game",
	"Help",
	"About",
	"Quit",
	"Continue"
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
	m_gameState(GameState::Undefined), 
	m_playerCountdown(0),
	m_option(App::TitleOptions::New)
{
}

App::~App()
{
}

int32_t App::initialize(const std::vector<std::string>& argList)
{
	this->m_gameState = GameState::Initializing;

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
		al_convert_mask_to_alpha(icon, al_map_rgb(255, 0, 255));
		al_set_display_icon(this->m_display, icon);
		al_destroy_bitmap(icon);
	}
	al_set_window_title(this->m_display, App::appTitle.c_str());

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

	al_start_timer(this->m_timer);

	Theme::setTheme(Theme::ThemeOptions::ThemeWedding);

	this->m_gameState = GameState::TitleScreen;
	std::cout << "Initialization Complete" << std::endl << std::endl;
	
	return 0;
}

void App::shutdown()
{
	std::cout << std::endl << "Shutdown Begin" << std::endl;
	this->m_gameState = GameState::Shuttingdown;

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

void App::draw() const
{

	al_set_target_bitmap(this->m_buffer);

	al_clear_to_color(Theme::getColor(Theme::Color::Background));

	if (this->m_gameState == GameState::TitleScreen || 
		this->m_gameState == GameState::HelpScreen ||
		this->m_gameState == GameState::AboutScreen)
	{
		this->drawTitle();
	}
	else
	{
		this->drawGame();
	}
}

void App::drawGame() const
{
	int32_t x = this->m_input.getX();
	int32_t y = this->m_input.getY();

	if (this->m_gameState == GameState::PlayerXWin || 
		this->m_gameState == GameState::PlayerOWin || 
		this->m_gameState == GameState::Tie )
	{
		x = -100;
	}

	this->m_grid.drawGrid(x, y);
	this->drawStatus();
	al_set_target_backbuffer(this->m_display);
	al_draw_bitmap(this->m_buffer, 0, 0, 0);
	al_flip_display();
}

void App::drawTitle() const
{
	static const std::string prompt = "-> ";
	static const std::string by = "by ";

	static float w = static_cast<float>(App::screenWidth) / 3.0f;
	static float h = static_cast<float>(App::screenHeight) / 3.0f;

	static float bw = al_get_text_width(this->m_font, by.c_str());
	static float aw = al_get_text_width(this->m_font, App::author.c_str());
	static float bx = (w - (bw + aw)) / 2.0f;

	static ALLEGRO_BITMAP* b = Sprites::getSprite(Sprites::SpriteList::Title);
	static float dx = w * 0.1f;
	static float dy = h * 0.1f;
	static float dw = w * 0.8f;
	static float dh = h * 0.4f;

	static float th = static_cast<float>(al_get_font_line_height(this->m_font)) * 1.5f;
	static float tx = w / 2.0f;
	static float ty = dy + dh + th * 3.0f;

	al_draw_scaled_bitmap(b, 0, 0, al_get_bitmap_width(b), al_get_bitmap_height(b), dx, dy, dw, dh, 0);

	for (int32_t i = static_cast<int32_t>(App::TitleOptions::New); i <= static_cast<int32_t>(App::TitleOptions::Quit); ++i)
	{
		std::string text = App::optionText[i];
		if (i == static_cast<int32_t>(App::TitleOptions::New) && this->m_grid.isInPlay())
		{
			text = App::optionText[static_cast<int32_t>(App::TitleOptions::Continue)];
		}
		ALLEGRO_COLOR color = Theme::getColor(Theme::Color::Light);
		if (i == static_cast<int32_t>(this->m_option))
		{
			color = Theme::getColor(Theme::Color::Hover);
			al_draw_text(this->m_font, color, tx, ty + (static_cast<float>(i) * th), ALLEGRO_ALIGN_RIGHT, prompt.c_str());
		}
		al_draw_text(this->m_font, color, tx, ty + (static_cast<float>(i) * th), ALLEGRO_ALIGN_LEFT, text.c_str());
	}
	al_draw_text(this->m_font, Theme::getColor(Theme::Color::Light), bx, h - (static_cast<float>(al_get_font_line_height(this->m_font)) * 1.5f), ALLEGRO_ALIGN_LEFT, (by + App::author).c_str());

	if (this->m_gameState == GameState::HelpScreen)
	{
		this->drawHelp();
	}

	if (this->m_gameState == GameState::AboutScreen)
	{
		this->drawAbout();
	}

	al_set_target_backbuffer(this->m_display);
	al_draw_scaled_bitmap(this->m_buffer, 0, 0, w, h, 0, 0, static_cast<float>(App::screenWidth), static_cast<float>(App::screenHeight), 0);
	al_flip_display();
}

void App::drawAbout() const
{
	const int32_t lines = 3;
	const std::string text[lines] =
	{
		std::string("About ") + App::appTitle,
		"",
		std::string("by ") + App::author
	};
	static std::vector<std::string> v;

	v.clear();

	for (int32_t i = 0; i < lines; ++i)
	{
		v.push_back(text[i]);
	}

	this->drawTextBox(v);
}

void App::drawHelp() const
{
	const int32_t lines = 12;
	const std::string text[lines] =
	{
		App::appTitle + std::string(" Help:"),
		"",
		"[ or ] to change theme",
		"",
		"Take turns with the",
		"computer. Place your",
		"mark in an empty square.",
		"Win the game with 3 in", 
		"a row (row, column, or",
		"diagonal).",
		"",
		"Good luck!",
	};

	static std::vector<std::string> v;

	v.clear();

	for (int32_t i = 0; i < lines; ++i)
	{
		v.push_back(text[i]);
	}

	this->drawTextBox(v);
}


void App::drawStatus() const
{
	static const char* prompt = "Status: ";
	static float cw = static_cast<float>(al_get_text_width(this->m_font, "W"));
	static float ch = static_cast<float>(al_get_font_line_height(this->m_font));
	static float tx = cw / 2.0f;
	static float ty = static_cast<float>(this->gridHeight) + (ch / 2.0f);
	static float px = tx + static_cast<float>(al_get_text_width(this->m_font, prompt));
	static const std::string text[5] =
	{
		"Your Turn",
		"Computer Turn",
		"You Won",
		"Computer Won",
		"Cats Game"
	};

	int32_t t = static_cast<int32_t>(this->m_gameState) - static_cast<int32_t>(GameState::PlayerXTurn);

	al_draw_text(this->m_font, Theme::getColor(Theme::Color::Light), tx + 1.0f, ty + 1.0f, ALLEGRO_ALIGN_LEFT, prompt);
	al_draw_text(this->m_font, Theme::getColor(Theme::Color::Hover), tx, ty, ALLEGRO_ALIGN_LEFT, prompt);

	al_draw_text(this->m_font, Theme::getColor(Theme::Color::Light), px + 1.0f, ty + 1.0f, ALLEGRO_ALIGN_LEFT, text[t].c_str());
	al_draw_text(this->m_font, Theme::getColor(Theme::Color::Hover), px, ty, ALLEGRO_ALIGN_LEFT, text[t].c_str());
}

void App::doLogic()
{
	if (this->m_gameState == GameState::TitleScreen)
	{

	}
	else
	{
		if (this->m_playerCountdown > 0)
		{
			--this->m_playerCountdown;
		}

		if (this->m_gameState == GameState::PlayerOTurn && this->m_playerCountdown == 0)
		{
			int32_t cell = this->m_grid.findComputerMove();
			this->m_grid.setCell(cell, Grid::CellType::Computer);

			Grid::GridValue value = this->m_grid.evaluateGrid();

			if (value == Grid::GridValue::Undecided)
			{
				this->m_gameState = GameState::PlayerXTurn;
			}
			else
			{
				switch (value)
				{
				case Grid::GridValue::Computer: this->m_gameState = GameState::PlayerOWin; break;
				case Grid::GridValue::Human: this->m_gameState = GameState::PlayerXWin; break;
				default: this->m_gameState = GameState::Tie; break;
				}
			}

			this->m_dirty = true;
		}



		if (this->m_gameState == GameState::PlayerXTurn)
		{
			int32_t insideCell = this->m_grid.isInsideCell(this->m_input.getX(), this->m_input.getY());

			if (insideCell >=0 && this->m_grid.getCell(insideCell) == Grid::CellType::Empty)
			{
				if (this->m_input.getButton(Input::MouseButton::Left))
				{
					this->m_grid.setCell(insideCell, Grid::CellType::Human);

					Grid::GridValue value = this->m_grid.evaluateGrid();

					if (value == Grid::GridValue::Undecided)
					{
						this->m_gameState = GameState::PlayerOTurn;
						this->m_playerCountdown = 60;// (60 * (rand() % 3));
					}
					else
					{
						switch (value)
						{
						case Grid::GridValue::Computer: this->m_gameState = GameState::PlayerOWin; break;
						case Grid::GridValue::Human: this->m_gameState = GameState::PlayerXWin; break;
						default: this->m_gameState = GameState::Tie; break;
						}
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
			case GameState::PlayerXWin:
			case GameState::PlayerOWin:
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					this->m_grid.reset();
					this->m_gameState = GameState::TitleScreen;
				}
			} break;
			case GameState::PlayerXTurn:
			case GameState::PlayerOTurn:
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					this->m_gameState = GameState::TitleScreen;
				}
			} break;

			case GameState::HelpScreen:
			case GameState::AboutScreen:
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					this->m_gameState = GameState::TitleScreen;
				}
			} break;

			case GameState::TitleScreen:
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ENTER ||
					event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
				{
					switch (this->m_option)
					{
					case App::TitleOptions::New:
					{
						this->m_gameState = GameState::PlayerXTurn;
						al_show_mouse_cursor(this->m_display);
					} break;
					case App::TitleOptions::Help:
					{
						this->m_gameState = GameState::HelpScreen;
					} break;
					case App::TitleOptions::About:
					{
						this->m_gameState = GameState::AboutScreen;
					} break;
					case App::TitleOptions::Quit:
					{
						this->m_kill = true;
					} break;
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
						this->m_option = App::TitleOptions(static_cast<int32_t>(this->m_option) - 1);
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
						this->m_option = App::TitleOptions(static_cast<int32_t>(this->m_option) + 1);
					}
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					this->m_kill = true;
				}
			} break;
			case GameState::Tie:
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					this->m_gameState = GameState::TitleScreen;
					this->m_grid.reset();
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

void App::drawTextBox(const std::vector<std::string>& text) const
{
	const float LINE_THICKNESS = 2.0f;
	static float sw = static_cast<float>(App::screenWidth) / 3.0f;
	static float sh = static_cast<float>(App::screenHeight) / 3.0f;

	float fh = static_cast<float>(al_get_font_line_height(this->m_font));
	float fw = static_cast<float>(al_get_text_width(this->m_font, "W"));
	float cell_padding[4] =
	{
		fh * 0.5f,
		fh * 0.5f,
		fw * 0.5f,
		fw * 0.5f
	};
	float text_spacing = fh * 0.125;
	float tw = 0;
	float th = cell_padding[2] + cell_padding[3] + (static_cast<float>(text.size()) * fh) + (static_cast<float>(text.size() - 1) * text_spacing);

	for (int32_t i = 0; i < text.size(); ++i)
	{
		tw = std::max(tw, static_cast<float>(al_get_text_width(this->m_font, text[i].c_str())));
	}

	tw = tw + cell_padding[0] + cell_padding[1];

	float x = (sw - tw) / 2.0f;
	float y = (sh - th) / 2.0f;


	al_draw_filled_rectangle(x, y, (x + tw - 1), (y + th - 1), Theme::getColor(Theme::Color::Background));
	al_draw_line(x, y, (x + tw - 1), y, Theme::getColor(Theme::Color::Light), LINE_THICKNESS);
	al_draw_line(x, y, x, (y + th - 1), Theme::getColor(Theme::Color::Light), LINE_THICKNESS);
	al_draw_line(x, (y + th - 1), (x + tw - 1), (y + th - 1), Theme::getColor(Theme::Color::Dark), LINE_THICKNESS);
	al_draw_line((x + tw - 1), y, (x + tw - 1), (y + th - 1), Theme::getColor(Theme::Color::Dark), LINE_THICKNESS);

	for (int32_t i = 0; i < text.size(); ++i)
	{
		al_draw_text(this->m_font, Theme::getColor(Theme::Color::Hover), x + (tw * 0.5f), y + cell_padding[1] + (i * (fh + text_spacing)), ALLEGRO_ALIGN_CENTRE, text[i].c_str());
	}
}
