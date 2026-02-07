#include "constraints.hpp"
#include <ctime>
#include <istream>

std::istream&
course::detail::operator>>(std::istream& in, course::detail::delim_char&& rhs)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  char c{};
  in >> c;
  if (rhs.expected != c)
  {
    in.setstate(std::ios::badbit);
  }

  return in;
}

std::istream&
course::operator>>(std::istream& in, course::constraint1& lhs)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  using delim = course::detail::delim_char;
  constraint1 tmp{};
  in >> delim{ '{' } >> tmp.prop >> delim{ ',' } >> tmp.obj >> delim{ ',' } >>
    tmp.value >> delim{ '}' };

  if (in)
  {
    lhs = tmp;
  }

  return in;
}

std::istream&
course::operator>>(std::istream& in, course::obj_value& rhs)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  using delim = course::detail::delim_char;
  obj_value tmp{};
  in >> delim{ '{' } >> tmp.prop >> delim{ ',' } >> tmp.value >> delim{ '}' };

  if (in)
  {
    rhs = tmp;
  }

  return in;
}

std::istream&
course::operator>>(std::istream& in, course::constraint2& lhs)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }
  using delim = course::detail::delim_char;
  constraint2 tmp{};

  in >> delim{ '{' } >> tmp.first >> delim{ ',' } >> tmp.second >> delim{ '}' };

  if (in)
  {
    lhs = tmp;
  }

  return in;
}

std::istream&
course::operator>>(std::istream& in, course::constraint3& lhs)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  using delim = course::detail::delim_char;
  constraint3 tmp{};

  in >> delim{ '{' } >> tmp.side >> delim{ ',' } >> tmp.lhs >> delim{','} >> tmp.rhs >> delim{ '}' };

  if (in)
  {
    lhs = tmp;
  }
  return in;
}

std::istream&
course::operator>>(std::istream& in, course::constraint4& lhs)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  using delim = course::detail::delim_char;
  constraint4 tmp{};

  in >> delim{ '{' } >> tmp.first >> delim{ ',' } >> tmp.second >> delim{ '}' };

  if (in)
  {
    lhs = tmp;
  }
  return in;
}
