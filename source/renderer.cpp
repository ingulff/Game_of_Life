/**
 *  ᛝ
 */

#include <iostream>
#include <utility>

#include "color_palete.hpp"
#include "error/error.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "utils/board.hpp"
#include "utils/index_helpers.hpp"
#include "utils/sdl_renderer_ptr.hpp"
#include "utils/sdl_window_ptr.hpp"

namespace tt_program
{

class renderer::renderer_impl
{

public:
	renderer_impl()
		: m_settings(tt_program::make_default_settings())
		, m_window()
		, m_renderer()
	{}

	renderer_impl(renderer_impl & other) = delete;
	renderer_impl & operator=(renderer_impl & other) = delete;

	renderer_impl(renderer_impl && other)
		: m_window(std::move(other.m_window))
		, m_renderer(std::move(other.m_renderer))
	{}

	renderer_impl & operator=(renderer_impl && other)
	{
		m_window = std::move(m_window);
		m_renderer = std::move(other.m_renderer);
	}

	~renderer_impl()
	{}

public:
	enum class error::status_code initialize(tt_program::settings_t & settings)
	{
		m_window = tt_program::details::sdl_window_ptr("Life", 
				SDL_WINDOWPOS_CENTERED, 
				SDL_WINDOWPOS_CENTERED, 
				m_settings.window_width, 
				m_settings.window_height,
				0);
		m_renderer = tt_program::details::sdl_renderer_ptr(m_window.get(),  -1, SDL_RENDERER_PRESENTVSYNC);

		enum class error::status_code result = error::status_code::invalid;

		if( m_renderer && m_window )
		{
			m_settings = settings;
			result = error::status_code::paused;
		}

		return result;
	}

public:
	void update(tt_program::details::board_t & board)
	{
		fill_backgrownd();
		fill_cells(board);
		draw_backgrownd_lines();

		SDL_RenderPresent(m_renderer.get());
	}

private:
	void fill_backgrownd()
	{
		SDL_SetRenderDrawColor(m_renderer.get(), 
			colors::backgrownd_color.red, 
			colors::backgrownd_color.green, 
			colors::backgrownd_color.blue, 
			colors::backgrownd_color.alpha );

		SDL_RenderClear(m_renderer.get());		
	}

	void draw_backgrownd_lines()
	{
		SDL_SetRenderDrawColor(m_renderer.get(), 
			colors::lines_color.red, 
			colors::lines_color.green, 
			colors::lines_color.blue,
			colors::lines_color.alpha );

		for(std::int32_t i = 0; i <= m_settings.window_width; i += m_settings.cell_side)
		{

			SDL_RenderDrawLine(m_renderer.get(), i, 0, i, m_settings.window_height);
			SDL_RenderDrawLine(m_renderer.get(), 0, i, m_settings.window_width, i);
		}
	}

	void fill_cells(tt_program::details::board_t & board)
	{
		using tt_program::details::next_alive_cells_index;
		using tt_program::details::start_pos;

		tt_program::details::cooedinate_converter converter{m_settings.board_width, m_settings.board_height};
		std::int32_t board_size = (m_settings.board_width * m_settings.board_height) / 8 + 1;
		for(std::int32_t block_index = next_alive_cells_index(board, start_pos); 
			block_index < board_size; 
			block_index = next_alive_cells_index(board, block_index))
		{
			for(std::int8_t cell_index = 0; cell_index < 8; ++cell_index)
			{
				
				if( (board[block_index] >> cell_index) & 1 )
				{
					SDL_SetRenderDrawColor(m_renderer.get(), 
						colors::cell_color.red, 
						colors::cell_color.green, 
						colors::cell_color.blue,
						colors::cell_color.alpha );

					auto point = converter.coordinates_by_index(block_index, cell_index);
					SDL_Rect cell{ point.x * m_settings.cell_side, 
						point.y * m_settings.cell_side, 
						m_settings.cell_side, 
						m_settings.cell_side };
					draw_cell( cell );				
				}
			}
		}
	}

	void draw_cell(SDL_Rect & cell)
	{
		SDL_RenderFillRect(m_renderer.get(), &cell);
	}

public:
	void fullscreen_handle()
	{
		m_settings.is_fullscreen = !m_settings.is_fullscreen;
		if( SDL_SetWindowFullscreen(SDL_RenderGetWindow(m_renderer.get()), m_settings.is_fullscreen) < 0)
			std::cout << SDL_GetError();
	}

private:
	tt_program::settings_t m_settings;
	tt_program::details::sdl_window_ptr m_window;
	tt_program::details::sdl_renderer_ptr m_renderer;
};


renderer::renderer()
	: m_impl(std::make_unique<renderer_impl>())
{}

renderer::renderer(renderer && other)
	: m_impl(std::move(other.m_impl))
	, m_status(std::move(other.m_status))
{}

renderer & renderer::operator=(renderer && other)
{
	m_impl = std::move(other.m_impl);
	m_status = std::move(other.m_status);

	return *this;
}

renderer::~renderer()
{}


enum class error::status_code renderer::initialize(tt_program::settings_t & settings)
{
	m_status = m_impl->initialize(settings);

	return m_status;
}


void renderer::update(tt_program::details::board_t & board)
{
	m_impl->update(board);
}

void renderer::fullscreen_handle()
{
	m_impl->fullscreen_handle();
}

} // namespace tt_program