/**
 *  ᛝ
 */

#include <iostream>
#include <functional>
#include <utility>

#include <SDL_events.h>
#include <SDL_rect.h>

#include "error/error.hpp"
#include "io_interactor.hpp"
#include "utils/index_helpers.hpp"
#include "utils/sdl_mouse_handler.hpp"


namespace tt_program
{

class io_interactor::io_interactor_impl
{

public:
	io_interactor_impl()
		: m_event()
		, m_callbacks()
	{}

	io_interactor_impl(io_interactor_impl & other) = delete;
	io_interactor_impl & operator=(io_interactor_impl & other) = delete;

	io_interactor_impl(io_interactor_impl && other)
		: m_event(std::move(other.m_event))
		, m_callbacks(std::move(other.m_callbacks))
	{}

	io_interactor_impl & operator=(io_interactor_impl && other)
	{
		m_event = std::move(other.m_event);
		m_callbacks = std::move(other.m_callbacks);

		return *this;
	}

	~io_interactor_impl()
	{}

public:
	enum class error::status_code  initialize(callbacks_t callbacks)
	{
		m_callbacks = std::move(callbacks);
		return error::status_code::paused; // start state
	}

public:
	void update()
	{
		using tt_program::details::to_mouse_button_type;

		auto mouse_status = tt_program::details::mouse_handle();
		auto mouse_point = calculate_cell_coordinates(mouse_status);
		if( m_callbacks.chagne_cell_handle )
		{
			if( tt_program::details::is_left_clicked(to_mouse_button_type(mouse_status.status))  )
			{
				m_callbacks.chagne_cell_handle(mouse_point, tt_program::details::cell_state::alive);
			}

			if( tt_program::details::is_right_clicked(to_mouse_button_type(mouse_status.status)) )
			{
				m_callbacks.chagne_cell_handle(mouse_point, tt_program::details::cell_state::killed);
			}
			
		}

		if(SDL_PollEvent(&m_event))
		{
			switch(m_event.type)
			{
			case SDL_KEYDOWN:
				key_interact();
				break;
			case SDL_QUIT:
				m_callbacks.quit_handle();
				break;
			default:
				// nothink
				break;
			}
		}
	}

private:
	void key_interact()
	{
		switch(m_event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			if(m_callbacks.quit_handle)
			{
				m_callbacks.quit_handle();
			}
			break;

		case SDLK_SPACE:
			if(m_callbacks.pause_handle)
			{
				m_callbacks.pause_handle();
			}
			break;

		default:
			// nothink
			break;
		}
	}

private:
	tt_program::details::point_t calculate_cell_coordinates(tt_program::details::mouse_t & mouse_status)
	{
		tt_program::details::point_t mouse_point;
		mouse_point.x = mouse_status.pos_x / 20;
		mouse_point.y = mouse_status.pos_y / 20;

		return mouse_point;
	}

private:
	SDL_Event m_event;
	callbacks_t m_callbacks;
};


io_interactor::io_interactor()
	: m_impl(std::make_unique<io_interactor_impl>())
{}

io_interactor::io_interactor(io_interactor && other) = default;
io_interactor & io_interactor::operator=(io_interactor && other) = default;

io_interactor::~io_interactor()
{}


enum class error::status_code io_interactor::initialize(callbacks_t callbacks)
{
	return m_impl->initialize( std::move(callbacks) );
}


void io_interactor::update()
{
	m_impl->update();
}


callbacks_t make_callbacks(std::function<void()> quit_handle, 
	std::function<void()> pause_handle,
	std::function<void(tt_program::details::point_t mouse_point, tt_program::details::cell_state)> chagne_cell_handle)
{
	return { std::move(quit_handle), std::move(pause_handle), std::move(chagne_cell_handle)};
}

} // namespace tt_program
