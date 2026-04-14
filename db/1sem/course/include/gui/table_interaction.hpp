#ifndef TABLE_INTERACTION_HPP
#define TABLE_INTERACTION_HPP

#include <string>
#include <vector>
#include "table_operations.hpp"
namespace course
{
  struct InputFieldLabel
  {
    std::string label;

    InputFieldLabel(const std::string& rhs) : label(rhs) {}
  };

  void insert_popup(pqxx::connection& c,
                    const std::string& table,
                    const std::vector< std::string >& labels);
  void update_popup(pqxx::connection& c,
                    const std::string& table,
                    const std::vector< std::string >& labels);
  void delete_popup(pqxx::connection& c,
                    const std::string& table,
                    const std::vector< std::string >& labels);
}

#endif
