#ifndef TT_INDEX_HELPERS_HPP
#define TT_INDEX_HELPERS_HPP

#include <cstdint>

#include "utils/board.hpp"

namespace tt_program::details
{

struct point_t
{
	std::int32_t x;
	std::int32_t y;
};

std::int32_t index_by_coordinates(std::int32_t x, std::int32_t y);

std::int32_t bit_index_by_coordinates(std::int32_t x, std::int32_t y);


std::int32_t next_alive_cells_index(board_t & board, std::int32_t last_index);

struct point_t coordinates_by_index(std::int32_t index, std::int8_t cell_index);

} // namespace tt_program::details

#endif // TT_INDEX_HELPERS_HPP
