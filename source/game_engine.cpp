/**
 *  ᛝ
 */
#include <iostream>
#include <memory>
#include <set>
#include <utility>

#include <cstdint>

#include "error/error.hpp"
#include "game_engine.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "utils/board.hpp"
#include "utils/index_helpers.hpp"
#include "utils/sdl_mouse_handler.hpp"

namespace tt_program
{

class engine::engine_impl
{

private:
	using board_t = tt_program::details::board_t;
	using point_t = tt_program::details::point_t;

public:
	engine_impl()
		: m_settings(tt_program::make_default_settings())
		, m_renderer()
		, m_board(nullptr)
		, m_reserve_board(nullptr)
	{}


public:
	enum class error::status_code initialize()
	{
		auto status = m_renderer.initialize(m_settings);
		if( is_initialized(status) )
		{
			std::int32_t board_size = (m_settings.board_width * m_settings.board_height) / 8 + 1;
			m_board = board_t(new std::int8_t[board_size]{0});
			m_reserve_board = board_t(new std::int8_t[board_size]{0});

		}
		
		return error::status_code::paused;
	}

public:
	void update(error::status_code & status)
	{
		if( is_playable(status) )
		{
			clear_board(m_reserve_board);
			calculate_next_step();

			swap(m_board, m_reserve_board);
		}

		m_renderer.update(m_board);
	}

private:
	bool is_initialized(error::status_code & status)
	{
		return status == error::status_code::paused;
	}

	bool is_playable(error::status_code & status)
	{
		return status == error::status_code::active;
	}

private:
	void clear_board(board_t & board)
	{
		for(std::int32_t i = 0, size = (m_settings.board_width * m_settings.board_height)/ 8 + 1; i < size; ++i)
		{
			board[i] = 0;
		}
	}

private:
	void calculate_next_step()
	{;

		tt_program::details::cooedinate_converter converter{m_settings.board_width, m_settings.board_height};
		std::int32_t board_size = (m_settings.board_width * m_settings.board_height) / 8 + 1;
		for(std::int32_t cell_group_index = 0; 
			cell_group_index < board_size; 
			++cell_group_index)
		{
			for(std::int8_t cell_index = 0; cell_index < 8; ++cell_index)
			{
				auto point = converter.coordinates_by_index(cell_group_index, cell_index );
				auto neighbors = (m_settings.is_loop_board) ? calclulate_neighbors_loop(point) : calclulate_neighbors(point);
				if( will_be_aline(point, neighbors))
				{

					m_reserve_board[converter.coordinates_to_block_index(point)] |= (1 << converter.coordinates_to_cell_index(point));
				}
				else if(will_and_was_alive(point, neighbors))
				{
					m_reserve_board[converter.coordinates_to_block_index(point)] |= (1 << converter.coordinates_to_cell_index(point));
				}
				else
				{
					m_reserve_board[converter.coordinates_to_block_index(point)] &= ~(1 << converter.coordinates_to_cell_index(point));
				}
			}
		}
	}

private:

	bool will_be_aline(tt_program::details::point_t & point, std::int32_t neighbors)
	{
 
		return ( (neighbors == 3) && !is_cell_alive(point.x, point.y) );
 	}

 	bool will_and_was_alive(tt_program::details::point_t & point, std::int32_t neighbors)
 	{
 		return ( (neighbors == 2 || neighbors == 3) && is_cell_alive(point.x, point.y) );
 	}


	std::int8_t calclulate_neighbors(tt_program::details::point_t & point)
	{
		std::int32_t prev_x = (point.x - 1) < 0 ? m_settings.board_width-1 : point.x - 1;
		std::int32_t next_x = (point.x + 1) % m_settings.board_width;
		std::int32_t prev_y = (point.y - 1) < 0 ? m_settings.board_height-1 : point.y - 1;
		std::int32_t next_y = (point.y + 1) % m_settings.board_height;
		
		std::int8_t neighbors = 0;
		
		bool has_prev_x = (point.x - 1 > -1);
		bool has_next_x = (point.x + 1 < m_settings.board_width);

		if( point.y - 1 > -1 )
		{
			if( has_prev_x )
			{
				neighbors += is_cell_alive(prev_x, prev_y);
			}

			neighbors += is_cell_alive(point.x, prev_y);

			if( has_next_x )
			{
				neighbors += is_cell_alive(next_x, prev_y);
			}
		}

		if( point.y + 1 < m_settings.board_height )
		{
			if( has_prev_x )
			{
				neighbors += is_cell_alive(prev_x, next_y);
			}

			neighbors += is_cell_alive(point.x, next_y);

			if( has_next_x )
			{
				neighbors += is_cell_alive(next_x, next_y);
			}
		}

		if( has_prev_x )
		{
			neighbors += is_cell_alive(prev_x, point.y);
		}

		if( has_next_x )
		{
			neighbors += is_cell_alive(next_x, point.y);
		}

		return neighbors;
	}

	std::int8_t calclulate_neighbors_loop(tt_program::details::point_t & point)
	{
		std::int32_t prev_x = (point.x - 1) < 0 ? m_settings.board_width-1 : point.x - 1;
		std::int32_t next_x = (point.x + 1) % m_settings.board_width;
		std::int32_t prev_y = (point.y - 1) < 0 ? m_settings.board_height-1 : point.y - 1;
		std::int32_t next_y = (point.y + 1) % m_settings.board_height;

		std::int8_t neighbors = 0;
		neighbors += is_cell_alive(prev_x, prev_y);
		neighbors += is_cell_alive(prev_x, point.y);
		neighbors += is_cell_alive(prev_x, next_y);
		neighbors += is_cell_alive(point.x, prev_y);
		neighbors += is_cell_alive(point.x, next_y);
		neighbors += is_cell_alive(next_x, prev_y);
		neighbors += is_cell_alive(next_x, point.y);
		neighbors += is_cell_alive(next_x, next_y);
		

		return neighbors;
	}

	bool is_cell_alive(std::int32_t x, std::int32_t y)
	{
		tt_program::details::cooedinate_converter converter{m_settings.board_width, m_settings.board_height};
		tt_program::details::point_t point{x, y};

		bool is_alive = ( ( m_board[converter.coordinates_to_block_index(point)] >> converter.coordinates_to_cell_index(point) )  & 1 );
 
		return  ( ( m_board[converter.coordinates_to_block_index(point)] >> converter.coordinates_to_cell_index(point) )  & 1 );

	}

public:
	void change_cell_handle(point_t mouse_position, tt_program::details::cell_state cell_status)
	{
		mouse_position.x /= m_settings.cell_side;
		mouse_position.y /= m_settings.cell_side;

		if(cell_status == tt_program::details::cell_state::killed)
		{
			keep_cell_dead(mouse_position);
		}

		if(cell_status == tt_program::details::cell_state::alive)
		{
			keep_cell_alive(mouse_position);
		}
	}

	void clear_board_handle()
	{
		clear_board(m_reserve_board);
		clear_board(m_board);
	}

	void loop_board_handle()
	{
		m_settings.is_loop_board = !m_settings.is_loop_board;
	}

	void fullscreen_handle()
	{
		m_renderer.fullscreen_handle();
	}

private:
	void keep_cell_alive(point_t & point)
	{
		tt_program::details::cooedinate_converter converter{m_settings.board_width, m_settings.board_height};

		m_board[converter.coordinates_to_block_index(point)] |= (1 << converter.coordinates_to_cell_index(point));
	}

	void keep_cell_dead(point_t & point)
	{
		tt_program::details::cooedinate_converter converter{m_settings.board_width, m_settings.board_height};

		m_board[converter.coordinates_to_block_index(point)] &= ~(1 << converter.coordinates_to_cell_index(point));
	}

private:
	tt_program::settings_t m_settings;
	tt_program::renderer m_renderer;
	board_t m_board;
	board_t m_reserve_board;
};


engine::engine()
	: m_impl(std::make_unique<engine_impl>())
{}

engine::engine(engine && other) = default;
engine & engine::operator=(engine && other) = default;

engine::~engine()
{}


enum class error::status_code engine::initialize()
{
	return m_impl->initialize();
}


void engine::update(error::status_code & status)
{
	m_impl->update(status);
}


void engine::change_cell_handle(tt_program::details::point_t mouse_position, tt_program::details::cell_state cell_status)
{
	m_impl->change_cell_handle(mouse_position, cell_status);
}

void engine::clear_board_handle()
{
	m_impl->clear_board_handle();
}

void engine::loop_board_handle()
{
	m_impl->loop_board_handle();
}

void engine::fullscreen_handle()
{
	m_impl->fullscreen_handle();
}

} // namespace tt_program
