/**
 *  ᛝ
 */

#include <memory>
#include <set>
#include <utility>

#include <cstdint>

#include "error/error.hpp"
#include "game_engine.hpp"
#include "renderer.hpp"
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
		: m_renderer()
		, m_board_width(0)
		, m_board_height(0)
		, m_board(nullptr)
		, m_is_loop_board(false)
	{}


public:
	enum class error::status_code initialize(std::int32_t width, std::int32_t height)
	{
		auto status = m_renderer.initialize();
		if( is_initialized(status) )
		{
			m_board_width = width;
			m_board_height = height;
			std::int32_t board_size = ((width * height) / 20 + 1) / 8 + 1;
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
		for(std::int32_t i = 0, size = ((m_board_width * m_board_height) / 20 + 1) / 8 + 1;
			i < size; ++i)
		{
			board[i] = 0;
		}
	}

private:
	void calculate_next_step()
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;
		using tt_program::details::coordinates_by_index;

		//std::int32_t block_index = start_pos;
		std::int32_t board_size = ((1000 / 20) * (1000 / 20) + 1) / 8 + 1;
		// находим блок, где есть изменненный бит, бит == селл
		for(std::int32_t cell_group_index = 0; 
			cell_group_index < board_size; 
			++cell_group_index)
		{
			for(std::int8_t bit_index = 0; bit_index < 8; ++bit_index)
			{
				auto [x, y] = coordinates_by_index(cell_group_index, bit_index );
				auto neighbors = (m_is_loop_board) ? calclulate_neighbors_loop(x, y) : calclulate_neighbors(x, y);
				if( will_be_aline(x, y, neighbors))
				{

					m_reserve_board[index_by_coordinates(x, y)] |= (1 << bit_index_by_coordinates(x, y));
				}
				else if(will_and_was_alive(x, y, neighbors))
				{
					m_reserve_board[index_by_coordinates(x, y)] |= (1 << bit_index_by_coordinates(x, y));
				}
				else
				{
					m_reserve_board[index_by_coordinates(x, y)] &= ~(1 << bit_index_by_coordinates(x, y));
				}
			}
		}
	}

private:

	bool will_be_aline(std::int32_t x, std::int32_t y, std::int32_t neighbors)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;
 
		return ( (neighbors == 3) && !is_cell_alive(x, y) );
 	}

 	bool will_and_was_alive(std::int32_t x, std::int32_t y, std::int32_t neighbors)
 	{
 		return ( (neighbors == 2 || neighbors == 3) && is_cell_alive(x, y) );
 	}


	std::int8_t calclulate_neighbors(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;

		std::int8_t neighbors = 0;
		bool has_prev_x = (x - 1 > -1);
		bool has_next_x = (x + 1 < m_board_width / 20);
		if(y - 1 > -1)
		{
			if( has_prev_x )
			{
				neighbors += is_cell_alive(x-1, y-1);
			}

			neighbors += is_cell_alive(x, y-1);

			if( has_next_x )
			{
				neighbors += is_cell_alive(x+1, y-1);
			}
		}

		if(y + 1 < m_board_height / 20)
		{
			if( has_prev_x )
			{
				neighbors += is_cell_alive(x-1, y+1);
			}

			neighbors += is_cell_alive(x, y+1);

			if( has_next_x )
			{
				neighbors += is_cell_alive(x+1, y+1);
			}
		}

		if( has_prev_x )
		{
			neighbors += is_cell_alive(x-1, y);
		}

		if( has_next_x )
		{
			neighbors += is_cell_alive(x+1, y);
		}

		return neighbors;
	}

	std::int8_t calclulate_neighbors_loop(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;

		std::int8_t neighbors = 0;
		std::int32_t prev_x = (x - 1) < 0 ? 49 : x - 1;
		std::int32_t next_x = (x + 1) % 50;
		std::int32_t prev_y = (y - 1) < 0 ? 49 : y - 1;
		std::int32_t next_y = (y + 1) % 50;

		neighbors += is_cell_alive(prev_x, prev_y);
		neighbors += is_cell_alive(prev_x, y);
		neighbors += is_cell_alive(prev_x, next_y);
		neighbors += is_cell_alive(x, prev_y);
		neighbors += is_cell_alive(x, next_y);
		neighbors += is_cell_alive(next_x, prev_y);
		neighbors += is_cell_alive(next_x, y);
		neighbors += is_cell_alive(next_x, next_y);
		

		return neighbors;
	}

	bool is_cell_alive(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;

		bool is_alive = ( ( m_board[index_by_coordinates(x, y)] >> bit_index_by_coordinates(x, y) )  & 1 );
 
		return  ( ( m_board[index_by_coordinates(x, y)] >> bit_index_by_coordinates(x, y) )  & 1 );

	}

public:
	void change_cell_handle(point_t & mouse_position, tt_program::details::cell_state cell_status)
	{
		if(cell_status == tt_program::details::cell_state::killed)
		{
			keep_cell_dead(mouse_position.x, mouse_position.y);
		}

		if(cell_status == tt_program::details::cell_state::alive)
		{
			keep_cell_alive(mouse_position.x, mouse_position.y);
		}
	}

	void clear_board_handle()
	{
		clear_board(m_reserve_board);
		clear_board(m_board);
	}

	void loop_board_handle()
	{
		m_is_loop_board = !m_is_loop_board;
	}

private:
	void keep_cell_alive(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;

		auto index = index_by_coordinates(x, y);
		auto bit = bit_index_by_coordinates(x, y);

		m_board[index_by_coordinates(x, y)] |= (1 << bit_index_by_coordinates(x, y));
	}

	void keep_cell_dead(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;
		
		auto index = index_by_coordinates(x, y);
		auto bit = bit_index_by_coordinates(x, y);

		m_board[index_by_coordinates(x, y)] &= ~(1 << bit_index_by_coordinates(x, y));
	}

private:
	tt_program::renderer m_renderer;
	std::int32_t m_board_width;
	std::int32_t m_board_height;
	board_t m_board;
	board_t m_reserve_board;
	bool m_is_loop_board;
};


engine::engine()
	: m_impl(std::make_unique<engine_impl>())
{}

engine::engine(engine && other) = default;
engine & engine::operator=(engine && other) = default;

engine::~engine()
{}


enum class error::status_code engine::initialize(std::int32_t width, std::int32_t height)
{
	return m_impl->initialize(width, height);
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

} // namespace tt_program
