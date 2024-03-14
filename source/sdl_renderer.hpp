#ifndef TT_SDL_RENDERER_HPP
#define TT_SDL_RENDERER_HPP

#include <memory>

#include "error/error.hpp"
#include "utils/sdl_window_ptr.hpp"

namespace tt_program
{

class sdl_renderer
{
public:
	sdl_renderer();

	sdl_renderer(sdl_renderer & other) = delete;
	sdl_renderer & operator=(sdl_renderer & other) = delete;

	sdl_renderer(sdl_renderer && other);
	sdl_renderer & operator=(sdl_renderer && other);

	~sdl_renderer();

public:
	enum class error::status_code initialize(tt_program::details::sdl_window_ptr & window_ptr, int index, Uint32 flags);

private:
	class sdl_renderer_impl;
	std::unique_ptr<sdl_renderer_impl> m_impl;

	error::status_code m_status;
};

} // namespace tt_program

#endif // TT_SDL_RENDERER_HPP
