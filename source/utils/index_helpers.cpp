/**
 *  ᛝ
 */

#include <iostream>
#include <vector>

#include <cstdint>

#include "utils/board.hpp"
#include "utils/index_helpers.hpp"

namespace tt_program::details
{

// if you change this function also you need check coordinates_by_index works correctly
std::int32_t index_by_coordinates(std::int32_t x, std::int32_t y)
{
	return ( y * (1000 / 20) + x ) / 8; 
}

std::int32_t bit_index_by_coordinates(std::int32_t x, std::int32_t y)
{
	return ( y * (1000 / 20) + x ) % 8;
}


std::int32_t next_alive_cells_index(board_t & board, std::int32_t last_index)
{
	auto new_index = last_index +1;
	std::int32_t board_size = ((1000 / 20 ) * (1000 / 20) + 1) / 8 + 1;
	for(new_index = last_index + 1; new_index < board_size; ++new_index)
	{
		if( board[new_index] )
		{
			break;
		}
	}

	return new_index;
}

// if you change this function also you need check index_by_coordinates works correctly
point_t coordinates_by_index(std::int32_t group_index, std::int8_t cell_index)
{
	auto coordinates_combination = group_index * 8 + cell_index;
	std::int32_t y = coordinates_combination / (1000 / 20);
	std::int32_t x = coordinates_combination % (1000/ 20);

	return { x, y }; 
}

} // namespace tt_program::details
