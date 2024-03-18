/**
 *  ᛝ
 */

#include <iostream> // delete
#include <utility>

#include <SDL.h>
#undef main

#include "error/error.hpp"
#include "game_engine.hpp"
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
		, m_engine()
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
		, m_engine(std::move(other.m_engine))
		, m_io_interactor(std::move(other.m_io_interactor))
		, m_status(std::move(other.m_status))
	{}

	main_window_impl & operator=(main_window_impl && other)
	{
		m_main_window = std::move(other.m_main_window);
		m_renderer = std::move(other.m_renderer);
		m_engine = std::move(other.m_engine);
		m_io_interactor = std::move(other.m_io_interactor);
		m_status = std::move(other.m_status);
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
	void initialize()
	{
			
		if( m_main_window )
		{
			auto renderer_status = m_renderer.initialize(m_main_window, -1, SDL_RENDERER_PRESENTVSYNC);
			auto engine_status = m_engine.initialize(1000, 1000);

			auto quit_handle_callback = [this]()
			{
				this->quit_handle();
			};
			auto pause_handle_callback = [this]()
			{
				this->pause_handle();
			};
			auto change_cell_handler_callback = [this](tt_program::details::point_t mouse_status, tt_program::details::cell_state cell_status)
			{
				this->m_engine.change_cell_handle( std::move(mouse_status), cell_status);
			};
			auto clear_board_hanler_callback = [this]()
			{
				this->m_engine.clear_board_handle();
			};
			auto fullscreen_handler_callback = [this]()
			{
				this->m_renderer.fullscreen_handle();
			};
			auto loop_board_handler_callback = [this]()
			{
				this->m_engine.loop_board_handle();
			};
			auto interactor_status = m_io_interactor.initialize(tt_program::make_callbacks(quit_handle_callback, 
				pause_handle_callback, 
				change_cell_handler_callback,
				clear_board_hanler_callback,
				fullscreen_handler_callback,
				loop_board_handler_callback));

			bool is_init = is_initialized(renderer_status) 
				&& is_initialized(engine_status)
				&& is_initialized(interactor_status);

			if( is_init )
			{
				m_status = error::status_code::paused;
			}
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

	bool is_initialized(error::status_code & status)
	{
		return status == error::status_code::paused;
	}

public:
	void quit_handle()
	{
		set_status(error::status_code::stopped);
	}

	void pause_handle()
	{
		if(m_status == error::status_code::paused)
		{
			set_status(error::status_code::active);
		}
		else
		{
			set_status(error::status_code::paused);
		}
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
		
			m_io_interactor.update();
			auto board = m_engine.update(m_status);
			m_renderer.update(board);

		}

		return error::to_errc(m_status);
	}

private:
	tt_program::details::sdl_window_ptr m_main_window;
	tt_program::sdl_renderer m_renderer;
	tt_program::engine m_engine;
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
