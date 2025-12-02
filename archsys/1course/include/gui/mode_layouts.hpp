#ifndef MODE_LAYOUTS_HPP
#define MODE_LAYOUTS_HPP

#include "configuration.hpp"

namespace course
{
  void call_automatic_mode_layout(const ProgramState &state, const ProgramConfiguration &config);
  void call_interactive_mode_layout(const ProgramState &state);

} // namespace course

#endif
