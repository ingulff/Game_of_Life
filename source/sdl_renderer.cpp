/**
 *  ᛝ
 */

#include <utility>

#include "error/error.hpp"
#include "sdl_renderer.hpp"
#include "utils/sdl_renderer_ptr.hpp"
#include "utils/sdl_window_ptr.hpp"

namespace tt_program
{

class sdl_renderer::sdl_renderer_impl
{

public:
	sdl_renderer_impl()
		: m_renderer()
	{}

	sdl_renderer_impl(sdl_renderer_impl & other) = delete;
	sdl_renderer_impl & operator=(sdl_renderer_impl & other) = delete;

	sdl_renderer_impl(sdl_renderer_impl && other)
		: m_renderer(std::move(other.m_renderer))
	{}

	sdl_renderer_impl & operator=(sdl_renderer_impl && other)
	{
		m_renderer = std::move(other.m_renderer);
	}

	~sdl_renderer_impl()
	{}

public:
	enum class error::status_code initialize(tt_program::details::sdl_window_ptr & window_ptr, int index, Uint32 flags)
	{
		m_renderer = tt_program::details::sdl_renderer_ptr(window_ptr, index, flags);

		enum class error::status_code result = error::status_code::invalid;

		if( m_renderer )
		{
			result = error::status_code::normal;
		}

		return result;
	}

private:
	tt_program::details::sdl_renderer_ptr m_renderer;
};


sdl_renderer::sdl_renderer()
	: m_impl(std::make_unique<sdl_renderer_impl>())
{}

sdl_renderer::sdl_renderer(sdl_renderer && other)
	: m_impl(std::move(other.m_impl))
	, m_status(std::move(other.m_status))
{}

sdl_renderer & sdl_renderer::operator=(sdl_renderer && other)
{
	m_impl = std::move(other.m_impl);
	m_status = std::move(other.m_status);

	return *this;
}

sdl_renderer::~sdl_renderer()
{}


enum class error::status_code sdl_renderer::initialize(tt_program::details::sdl_window_ptr & window_ptr, int index, Uint32 flags)
{
	m_status = m_impl->initialize(window_ptr, index, flags);

	return m_status;
}

} // namespace tt_program