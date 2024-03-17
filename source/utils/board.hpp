#ifndef TT_BOARD_HPP
#define TT_BOARD_HPP

#include <memory>

#include <cstdint>

namespace tt_program::details
{

using board_t = std::shared_ptr<std::int8_t []>;

inline std::int32_t start_pos = -1; // чтобы всегда начинать со следующей от последнего индекса

enum class cell_state : std::int8_t
{
	killed = 0,
	alive = 1,
};

} // namespace tt_program::details

#endif // TT_BOARD_HPP
