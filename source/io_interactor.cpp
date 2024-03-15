/**
 *  ᛝ
 */

#include <functional>
#include <utility>

#include <SDL_events.h>

#include "error/error.hpp"
#include "io_interactor.hpp"


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
		if(SDL_PollEvent(&m_event))
		{
			switch(m_event.type)
			{
			case SDL_KEYDOWN:
				key_interact();
			}
		}
	}

private:
	void key_interact()
	{
		switch(m_event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			if(m_callbacks.quit_handler)
			{
				m_callbacks.quit_handler();
			}
			break;

		case SDLK_SPACE:
			if(m_callbacks.pause_handler)
			{
				m_callbacks.pause_handler();
			}
			break;

		default:
			// nothink
			break;
		}
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


callbacks_t make_callbacks(std::function<void()> quit_handler, 
	std::function<void()> pause_handler)
{
	return { std::move(quit_handler), std::move(pause_handler)};
}

} // namespace tt_program
