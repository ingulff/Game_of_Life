#ifndef TT_GAME_EMGINE_HPP
#define TT_GAME_EMGINE_HPP

#include <memory>

#include "error/error.hpp"
#include "utils/index_helpers.hpp"
#include "utils/sdl_mouse_handler.hpp"

namespace tt_program
{

class engine
{
public:
	engine();

	engine(engine & other) = delete;
	engine & operator=(engine & other) = delete;

	engine(engine && other);
	engine & operator=(engine && other);

	~engine();

public:
	enum class error::status_code initialize();

public:
	void update(error::status_code & status);

public:
	void change_cell_handle(tt_program::details::point_t, tt_program::details::cell_state cell_status);

	void clear_board_handle();

	void loop_board_handle();

private:
	class engine_impl;
	std::unique_ptr<engine_impl> m_impl;
};

} // namespace tt_program

#endif // TT_GAME_EMGINE_HPP
