/**
 *  ᛝ
 */

#include <iostream>
#include <utility>

#include "color_palete.hpp"
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
			result = error::status_code::active;
		}

		return result;
	}

public:
	void update()
	{
		fill_backgrownd();
		draw_backgrownd_lines();

		SDL_RenderPresent(m_renderer.get());
	}

private:
	void fill_backgrownd()
	{
		SDL_SetRenderDrawColor(m_renderer.get(), 
			colors::backgrownd_color.red, 
			colors::backgrownd_color.green, 
			colors::backgrownd_color.blue, 
			colors::backgrownd_color.alpha );

		SDL_RenderClear(m_renderer.get());		
	}

	void draw_backgrownd_lines()
	{
		SDL_SetRenderDrawColor(m_renderer.get(), 
			colors::lines_color.red, 
			colors::lines_color.green, 
			colors::lines_color.blue,
			colors::lines_color.alpha );

		for(int i = 0; i <= 1000; i += 20)
		{

			SDL_RenderDrawLine(m_renderer.get(), i, 0, i, 1000);
			SDL_RenderDrawLine(m_renderer.get(), 0, i, 1000, i);
		}
	}

public:
	void draw_cell(SDL_Rect & cell)
	{
		SDL_RenderFillRect(m_renderer.get(), &cell);
		SDL_RenderPresent(m_renderer.get());
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


void sdl_renderer::draw_cell(SDL_Rect & cell)
{
	m_impl->draw_cell(cell);
}


void sdl_renderer::update()
{
	m_impl->update();
}

} // namespace tt_program