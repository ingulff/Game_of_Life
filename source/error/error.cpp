/**
 *  ᛝ
 */

#include "error/error.hpp"

namespace error
{

enum class errc to_errc(status_code status) noexcept
{
	errc error;
	switch(status)
	{
	case status_code::active: // ?? (maybe it error)
	case status_code::stopped:
		error = errc::ok;
		break;
	case status_code::invalid:
		error = errc::invalid_initialization;
		break;
	default:
		error = errc::unknown;
	}

	return error;
}

int to_int(errc error_code) noexcept
{
	return static_cast<int>(error_code);
}

} // namespace error
