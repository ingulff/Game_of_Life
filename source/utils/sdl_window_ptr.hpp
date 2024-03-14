#ifndef TT_SDL_WINDOW_PTR_HPP
#define TT_SDL_WINDOW_PTR_HPP

#include <utility>

#include <SDL_video.h>

namespace tt_program::details
{

class sdl_window_ptr
{
public:
	sdl_window_ptr()
		: m_window(SDL_CreateWindow("Game of Life",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				1000,
				1000,
				0))
	{}

	sdl_window_ptr(sdl_window_ptr & other) = delete;
	sdl_window_ptr & operator=(sdl_window_ptr & other) = delete;

	sdl_window_ptr(sdl_window_ptr && other)
		: m_window(std::exchange(other.m_window, nullptr))
	{}

	sdl_window_ptr & operator=(sdl_window_ptr && other)
	{
		m_window = std::exchange(other.m_window, nullptr);
		return *this;
	}

	~sdl_window_ptr()
	{
		SDL_DestroyWindow(m_window);

	}

public:
	explicit operator bool() const noexcept
	{
		return m_window != nullptr;
	}

public:
	typename std::add_lvalue_reference<SDL_Window>::type operator*()
	{
		return *m_window;
	}

	SDL_Window * operator->()
	{
		return m_window;
	}

	SDL_Window * get()
	{
		return m_window;;
	}

private:
	SDL_Window * m_window;	
};

} // namespace tt_program::details

#endif // TT_SDL_WINDOW_PTR_HPP
