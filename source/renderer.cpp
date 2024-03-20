/**
 *  ᛝ
 */

#include <iostream>
#include <utility>

#include "color_palete.hpp"
#include "error/error.hpp"
#include "renderer.hpp"
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
		: m_renderer()
		, is_fullscreen(false)
	{}

	renderer_impl(renderer_impl & other) = delete;
	renderer_impl & operator=(renderer_impl & other) = delete;

	renderer_impl(renderer_impl && other)
		: m_renderer(std::move(other.m_renderer))
	{}

	renderer_impl & operator=(renderer_impl && other)
	{
		m_renderer = std::move(other.m_renderer);
	}

	~renderer_impl()
	{}

public:
	enum class error::status_code initialize(tt_program::details::sdl_window_ptr & window_ptr, int index, Uint32 flags)
	{
		m_renderer = tt_program::details::sdl_renderer_ptr(window_ptr, index, flags);

		enum class error::status_code result = error::status_code::invalid;

		if( m_renderer )
		{
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

		for(int i = 0; i <= 1000; i += 20)
		{

			SDL_RenderDrawLine(m_renderer.get(), i, 0, i, 1000);
			SDL_RenderDrawLine(m_renderer.get(), 0, i, 1000, i);
		}
	}

	void fill_cells(tt_program::details::board_t & board)
	{
		using tt_program::details::next_alive_cells_index;
		using tt_program::details::coordinates_by_index;
		using tt_program::details::start_pos;

		std::int32_t board_size = ((1000 / 20) * (1000 / 20) + 1) / 8 + 1;
		for(std::int32_t cell_group_index = next_alive_cells_index(board, start_pos); 
			cell_group_index < board_size; 
			cell_group_index = next_alive_cells_index(board, cell_group_index))
		{
			for(std::int8_t bit_index = 0; bit_index < 8; ++bit_index)
			{
				
				if( (board[cell_group_index] >> bit_index) & 1 )
				{
					SDL_SetRenderDrawColor(m_renderer.get(), 
						colors::cell_color.red, 
						colors::cell_color.green, 
						colors::cell_color.blue,
						colors::cell_color.alpha );

					auto point = coordinates_by_index(cell_group_index, bit_index);
					SDL_Rect cell{ point.x * 20, point.y * 20, 20, 20 };
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
		is_fullscreen = !is_fullscreen;
		SDL_SetWindowFullscreen(SDL_RenderGetWindow(m_renderer.get()), is_fullscreen);
	}

private:
	tt_program::details::sdl_renderer_ptr m_renderer;
	bool is_fullscreen;
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


enum class error::status_code renderer::initialize(tt_program::details::sdl_window_ptr & window_ptr, int index, Uint32 flags)
{
	m_status = m_impl->initialize(window_ptr, index, flags);

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