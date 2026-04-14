#ifndef CLICKABLE_TABLE_HPP
#define CLICKABLE_TABLE_HPP

#include <pqxx/pqxx>
#include <string>
namespace course
{
  void place_clickable_table(pqxx::connection& c,
                             const std::string& schema,
                             const std::string& table_name);
} // namespace course

#endif
