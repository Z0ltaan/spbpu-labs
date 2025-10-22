#ifndef ABSTRACTDISCIPLINE_HPP
#define ABSTRACTDISCIPLINE_HPP

#include "priority.hpp"

namespace course
{
  class Discipline
  {
  public:
    virtual priority_t getPriority() = 0;
    virtual ~Discipline() = default;
  };
} // namespace course
#endif
