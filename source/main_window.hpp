#ifndef TT_GAME_OF_LIFE_HPP
#define TT_GAME_OF_LIFE_HPP

#include <memory>

#include "error/error.hpp"

namespace tt_program
{

class main_window
{

public:
	main_window();

	main_window(main_window & other) = delete;
	main_window & operator=(main_window & other) = delete;

	main_window(main_window && other);
	main_window & operator=(main_window && other);

	~main_window();

public:
	int exec();

private:
	class main_window_impl;
	std::unique_ptr<main_window_impl> m_impl;
};

} // namespace tt_program


#endif // TT_GAME_OF_LIFE_HPP
