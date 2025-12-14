#ifndef WORK_ASSIGNMENT_TAB_HPP
#define WORK_ASSIGNMENT_TAB_HPP

#include <pqxx/pqxx>
#include <string>
#include <utility>

namespace course
{
  void place_work_assignment_tab(
    pqxx::connection& c,
    const std::vector< std::pair< std::string, std::string > >&
      table_column_pairs);
}

#endif
