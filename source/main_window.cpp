/**
 *  ᛝ
 */

#include <iostream> // delete
#include <utility>

#include <SDL.h>
#undef main

#include "error/error.hpp"
#include "main_window.hpp"
#include "sdl_renderer.hpp"
#include "utils/sdl_window_ptr.hpp"

namespace tt_program
{

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
		, m_renderer(std::move(other.m_renderer))
	{}

	main_window_impl & operator=(main_window_impl && other)
	{
		m_main_window = std::move(other.m_main_window);
		m_renderer = std::move(other.m_renderer);
	}

	~main_window_impl()
	{}

public:
	enum class error::status_code initialize()
	{
		enum class error::status_code result = error::status_code::invalid;
			
		if(m_main_window)
		{
			result = m_renderer.initialize(m_main_window, -1, SDL_RENDERER_PRESENTVSYNC);
		}

		return result;
	}

	void deinitialize()
	{}

private:
	tt_program::details::sdl_window_ptr m_main_window;
	tt_program::sdl_renderer m_renderer;
};


main_window::main_window()
	: m_impl(std::make_unique<main_window_impl>())
	, m_status(error::status_code::invalid)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_status = m_impl->initialize();
	}
	else
	{
std::cerr << "main_window::main_window() - sdl window not initialized" << std::endl;
	}
std::cout << "main_window::main_window()" << std::endl;
}

main_window::main_window(main_window && other) = default;
main_window & main_window::operator=(main_window && other) = default;

main_window::~main_window()
{
	m_impl->deinitialize();
	SDL_Quit();
std::cout << "main_window::~main_window()" << std::endl;
}


int main_window::exec()
{
std::cout << "main_window::exec()" << std::endl;
	while(true)
	{
		if(m_status != error::status_code::normal)
		{
std::cerr << "main_window::exec() - status of window not valid" << std::endl;
			break;
		}
		;
std::cout << "work" << std::endl;
	}

//	SDL_Quit();
	return 0;
}

} // namespace tt_program
