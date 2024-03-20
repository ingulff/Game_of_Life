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


// по доске проще итерироваться как по двумерному массиву (х, y)
// игровая доска имеет специфичный внутренний индекс
class cooedinate_converter
{

public:
	cooedinate_converter(std::int32_t width, std::int32_t height)
		: m_width(width)
		, m_height(height)
	{}

	std::int32_t coordinates_to_block_index(const struct point_t & point);

	std::int8_t coordinates_to_cell_index(const struct point_t & point);


	struct point_t coordinates_by_index(std::int32_t block_index, std::int8_t cell_index);


private:
	std::int32_t m_width;
	std::int32_t m_height;
};


std::int32_t next_alive_cells_index(board_t & board, std::int32_t last_index);


} // namespace tt_program::details

#endif // TT_INDEX_HELPERS_HPP
