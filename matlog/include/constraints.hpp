#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <array>
#include <csignal>
#include <cstddef>
#include <iosfwd>
#include <utility>

namespace course
{
  namespace detail
  {
    struct delim_char
    {
      char expected;
    };

    std::istream& operator>>(std::istream& in, detail::delim_char&& rhs);
  }

  struct obj_value
  {
    size_t prop;
    size_t value;
  };

  struct constraint1
  {
    size_t prop;
    size_t obj;
    size_t value;
  };

  struct constraint3
  {
    char side;
    obj_value lhs;
    obj_value rhs;
  };

  using constraint2 = std::pair< obj_value, obj_value >;
  using constraint4 = std::pair< constraint3, constraint3 >;

  std::istream& operator>>(std::istream& in, obj_value& lhs);
  std::istream& operator>>(std::istream& in, constraint1& lhs);
  std::istream& operator>>(std::istream& in, constraint2& lhs);
  std::istream& operator>>(std::istream& in, constraint3& lhs);
  std::istream& operator>>(std::istream& in, constraint4& lhs);

  template< class bdd, size_t obj_count, size_t prop_count >
  using props_array_t =
    std::array< std::array< std::array< bdd, obj_count >, obj_count >,
                prop_count >;

} // namespace course

#endif
