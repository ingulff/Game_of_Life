#ifndef TT_IO_INTERACTION_HPP
#define TT_IO_INTERACTION_HPP

#include <functional>
#include <memory>

#include "utils/index_helpers.hpp"
#include "utils/sdl_mouse_handler.hpp"

namespace tt_program
{

struct callbacks_t
{
	std::function<void()> quit_handle;
	std::function<void()> pause_handle;
	std::function<void(tt_program::details::point_t, tt_program::details::cell_state)> chagne_cell_handle;
	std::function<void()> clear_board_handle;
	std::function<void()> loop_board_handle;
	std::function<void()> fullscreen_handle;
};

callbacks_t make_callbacks(std::function<void()> quit_handler, 
	std::function<void()> pause_handler,
	std::function<void(tt_program::details::point_t, tt_program::details::cell_state)> chagne_cell_handle,
	std::function<void()> clear_board_handle,
	std::function<void()> loop_board_handle,
	std::function<void()> fullscreen_handle);

class io_interactor
{

public:
	io_interactor();

	io_interactor(io_interactor & other) = delete;
	io_interactor & operator=(io_interactor & other) = delete;

	io_interactor(io_interactor && other);
	io_interactor & operator=(io_interactor && other);

	~io_interactor();

public:
	enum class error::status_code  initialize(callbacks_t callbacks);

public:
	void update();

private:
	class io_interactor_impl;
	std::unique_ptr<io_interactor_impl> m_impl;
};

} // namespace tt_program

#endif // TT_IO_INTERACTION_HPP
