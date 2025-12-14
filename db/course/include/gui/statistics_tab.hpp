#ifndef STATISTICS_TABS_HPP
#define STATISTICS_TABS_HPP

#include "get_column_values.hpp"
namespace course
{
  void place_scores_table_tab(pqxx::connection& c);
  void place_costs_tab(pqxx::connection& c);
}

#endif
