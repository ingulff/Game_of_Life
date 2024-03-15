/**
 *  ᛝ
 */

#include <iostream> // delete
#include <utility>

#include <SDL.h>
#undef main

#include "error/error.hpp"
#include "io_interactor.hpp"
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
		, m_io_interactor()
		, m_status(error::status_code::invalid)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
		{
			initialize();
		}
	}

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
	{
		this->deinitialize();
	}

public:
	void set_status(error::status_code status)
	{
		m_status = status;
	}

public:
	void quit_handle()
	{
std::cout << "quit - true" << std::endl;
		set_status(error::status_code::stopped);
	}

	void pause_handle()
	{
		if(m_status == error::status_code::paused)
		{
std::cout << "play - true" << std::endl;
			set_status(error::status_code::active);
		}
		else
		{
std::cout << "play - false" << std::endl;
			set_status(error::status_code::paused);
		}
	}


public:
	void initialize()
	{
			
		if( m_main_window || m_status == error::status_code::active )
		{
			m_status = m_renderer.initialize(m_main_window, -1, SDL_RENDERER_PRESENTVSYNC);
		}

		
		if( m_status == error::status_code::active )
		{
			auto quit_handle_callback = [this]()
			{
				this->quit_handle();
			};
			auto pause_handle_callback = [this]()
			{
				this->pause_handle();
			};
			auto cell_draw_handler_callback = [this](SDL_Rect & cell)
			{
				this->m_renderer.draw_cell(cell);
			};

			m_status = m_io_interactor.initialize(tt_program::make_callbacks(quit_handle_callback, 
				pause_handle_callback, cell_draw_handler_callback));
		}

	}

	void deinitialize()
	{
		SDL_Quit();
	}

private:
	bool is_playable()
	{
		return m_status == error::status_code::active || m_status == error::status_code::paused;
	}

public:
	error::errc exec()
	{
		while(true)
		{
			if( !is_playable() )
			{
				break;
			}
		
			m_renderer.update();
			m_io_interactor.update();
		}

		return error::to_errc(m_status);
	}

private:
	tt_program::details::sdl_window_ptr m_main_window;
	tt_program::sdl_renderer m_renderer;
	tt_program::io_interactor m_io_interactor;

	enum class error::status_code m_status;
};


main_window::main_window()
	: m_impl(std::make_unique<main_window_impl>())
{}

main_window::main_window(main_window && other) = default;
main_window & main_window::operator=(main_window && other) = default;

main_window::~main_window()
{}


int main_window::exec()
{
	return error::to_int(m_impl->exec());
}

} // namespace tt_program
