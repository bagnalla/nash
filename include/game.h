#ifndef GAME_H
#define GAME_H

#include <functional>

namespace nash
{
  typedef std::function<double(int, int)> utility_fun_t;

  // A struct record type for specifying games to be solved.
  struct Game
  {
    Game(size_t p1_action_space_size, size_t p2_action_space_size,
	 utility_fun_t utility_function)
    {
      this->p1_action_space_size = p1_action_space_size;
      this->p2_action_space_size = p2_action_space_size;
      this->utility_function = utility_function;
    }
    size_t p1_action_space_size, p2_action_space_size;
    utility_fun_t utility_function;
  };
}

#endif
