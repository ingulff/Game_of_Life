#ifndef TT_ERROR_HPP
#define TT_ERROR_HPP

#include <cstdint>

namespace error
{

enum class status_code : std::int8_t
{
	active = 0x0000,
	invalid = 0x0001,
	stopped = 0x0002,
	paused = 0x0003,
};

enum class errc
{
	ok = 0x00000000,
	unknown = 0x00000001,
	invalid_initialization = 0x00000002,
};


enum class errc to_errc(status_code status) noexcept;

int to_int(errc error_code) noexcept;

} // namespace error

#endif // TT_ERROR_HPP
