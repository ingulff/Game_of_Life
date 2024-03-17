/**
 *  ᛝ
 */

#include <iostream>
#include <memory>
#include <utility>

#include <cstdint>

#include "error/error.hpp"
#include "game_engine.hpp"
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
		: m_board_width(0)
		, m_board_height(0)
		, m_board(nullptr)
		, m_reserve_board(nullptr)
	{}


public:
	enum class error::status_code initialize(std::int32_t width, std::int32_t height)
	{
		m_board_width = width;
		m_board_height = height;
		std::int32_t board_size = ((width * height) / 20 + 1) / 8 + 1;
		m_board = board_t(new std::int8_t[board_size]{0});
		m_reserve_board = board_t(new std::int8_t[board_size]{0});

		return error::status_code::paused;
	}

public:
	std::shared_ptr<std::int8_t[]> update()
	{
		//calculate_next_step();

		//swap(m_board, m_reserve_board);

		return m_board;
	}

private:
	void calculate_next_step()
	{

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

private:
	void keep_cell_alive(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;

		m_board[index_by_coordinates(x, y)] |= (1 << bit_index_by_coordinates(x, y));
	}

	void keep_cell_dead(std::int32_t x, std::int32_t y)
	{
		using tt_program::details::index_by_coordinates;
		using tt_program::details::bit_index_by_coordinates;
		
		m_board[index_by_coordinates(x, y)] &= ~(1 << bit_index_by_coordinates(x, y));
	}

private:
	std::int32_t m_board_width;
	std::int32_t m_board_height;
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


enum class error::status_code engine::initialize(std::int32_t width, std::int32_t height)
{
	return m_impl->initialize(width, height);
}


std::shared_ptr<std::int8_t[]> engine::update()
{
	return m_impl->update();
}


void engine::change_cell_handle(tt_program::details::point_t mouse_position, tt_program::details::cell_state cell_status)
{
	m_impl->change_cell_handle(mouse_position, cell_status);
}

} // namespace tt_program
