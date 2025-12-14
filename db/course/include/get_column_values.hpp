#ifndef GET_COLUMN_VALUE_HPP
#define GET_COLUMN_VALUE_HPP

#include <pqxx/pqxx>
#include <string>

namespace course
{
  int place_column_values_list(pqxx::connection& c,
                               const std::string& table,
                               const std::string& column,
                               int& list_pos,
                               int& id);
}

#endif
