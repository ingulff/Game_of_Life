/**
 *  ᛝ
 */

#include <utility>

#include <SDL.h>
#undef main

#include "main_window.hpp"

namespace tt_program
{

namespace details
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


class sdl_renderer_ptr
{
public:
	sdl_renderer_ptr()
		: m_renderer(nullptr)
	{}

	sdl_renderer_ptr(sdl_window_ptr & window_ptr, int index, Uint32 flags)
		: m_renderer(SDL_CreateRenderer(window_ptr.get(), -1, SDL_RENDERER_PRESENTVSYNC))
	{}

	sdl_renderer_ptr(sdl_renderer_ptr & other) = delete;
	sdl_renderer_ptr & operator=(sdl_renderer_ptr & other) = delete;

	sdl_renderer_ptr(sdl_renderer_ptr && other)
		: m_renderer(std::exchange(other.m_renderer, nullptr))
	{}

	sdl_renderer_ptr & operator=(sdl_renderer_ptr && other)
	{
		m_renderer = std::exchange(other.m_renderer, nullptr);
		return *this;
	}

	~sdl_renderer_ptr()
	{
		SDL_DestroyRenderer(m_renderer);
	}

public:
	explicit operator bool() const noexcept
	{
		return m_renderer != nullptr;
	}

public:
	typename std::add_lvalue_reference<SDL_Renderer>::type operator*()
	{
		return *m_renderer;
	}

	SDL_Renderer * operator->()
	{
		return m_renderer;
	}

	SDL_Renderer * get()
	{
		return m_renderer;
	}	

private:
	SDL_Renderer * m_renderer;
};

} // namespace details


class main_window::main_window_impl
{

public:
	main_window_impl()
		: m_main_window()
		, m_renderer()
	{}

	main_window_impl(main_window_impl & other) = delete;
	main_window_impl & operator=(main_window_impl & other) = delete;

	main_window_impl(main_window_impl && other)
		: m_main_window(std::move(other.m_main_window))
	{}

	main_window_impl & operator=(main_window_impl && other)
	{
		m_main_window = std::move(other.m_main_window);
	}

	~main_window_impl()
	{}

public:
	enum class main_window::status_code initialize()
	{
		main_window::status_code result = main_window::status_code::invalid;
			
		if(m_main_window)
		{
			m_renderer = tt_program::details::sdl_renderer_ptr(m_main_window, -1, SDL_RENDERER_PRESENTVSYNC);
			if(m_renderer)
			{
				result = main_window::status_code::normal;
			}
		}

		return result;
	}

	void deinitialize()
	{}

private:
	tt_program::details::sdl_window_ptr m_main_window;
	tt_program::details::sdl_renderer_ptr m_renderer;
};


main_window::main_window()
	: m_impl(std::make_unique<main_window_impl>())
	, m_status(status_code::invalid)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_status = m_impl->initialize();
	}
}

main_window::main_window(main_window && other) = default;
main_window & main_window::operator=(main_window && other) = default;

main_window::~main_window()
{
	m_impl->deinitialize();
	SDL_Quit();
}


int main_window::exec()
{
	while(true)  ;;
	return 0;
}

} // namespace tt_program
