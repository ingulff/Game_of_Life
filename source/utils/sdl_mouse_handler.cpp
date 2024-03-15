/**
 *  ᛝ
 */

#include <SDL.h>
#undef main

#include "utils/sdl_mouse_handler.hpp"

namespace tt_program::details
{

struct mouse_t mouse_handle()
{
	mouse_t mouse_status;
	mouse_status.status = SDL_GetMouseState(&mouse_status.pos_x, &mouse_status.pos_y);

	return mouse_status;
}


enum class mouse_button_t to_mouse_button_type(int type)
{
	enum class mouse_button_t button;
	switch(type)
	{
	case 1:
		button = mouse_button_t::left;
		break;
	case 2:
		button = mouse_button_t::right;
		break;
	case 3:
		button = mouse_button_t::middle;
		break;
	default:
		button = mouse_button_t::unknown;
	}

	return button;
}

} // namespace tt_program::details
