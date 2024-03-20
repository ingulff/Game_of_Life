/**
 *  ᛝ
 */

#include <iostream>

#include <cstdint>

#include "utils/board.hpp"
#include "utils/index_helpers.hpp"

namespace tt_program::details
{

// if you change this function also you need check coordinates_by_index works correctly
std::int32_t cooedinate_converter::coordinates_to_block_index(const struct point_t & point)
{
	return ( point.y * m_height + point.x ) / 8; 
}


std::int8_t cooedinate_converter::coordinates_to_cell_index(const struct point_t & point)
{
	return ( point.y * m_height + point.x ) % 8;
}


// if you change this function also you need check index_by_coordinates works correctly
struct point_t cooedinate_converter::coordinates_by_index(std::int32_t block_index, std::int8_t cell_index)
{

	auto coordinates_combination = block_index * 8 + cell_index;
	std::int32_t y = coordinates_combination / m_height;
	std::int32_t x = coordinates_combination % m_width;
	
	return { x, y }; 
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



} // namespace tt_program::details
