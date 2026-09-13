#ifndef TYPE_UTILS_HPP
#define TYPE_UTILS_HPP

#include <cctype>
#include <type_traits>

namespace cmilan
{

  template< class enumenator >
  constexpr inline auto to_underlying_type(enumenator value)
  {
    return static_cast< std::underlying_type_t< decltype(value) > >(value);
  }

  template< class char_t >
  bool is_alpha(char_t c)
  {
    return std::isalpha(static_cast< unsigned char >(c));
  }

  template< class char_t >
  bool is_digit(char_t c)
  {
    return std::isdigit(static_cast< unsigned char >(c));
  }
  template< class char_t >
  bool is_space(char_t c)
  {
    return std::isspace(static_cast< unsigned char >(c));
  }
  template< class char_t >
  char_t to_lower(char_t c)
  {
    return std::tolower(static_cast< unsigned char >(c));
  }
}

#endif
