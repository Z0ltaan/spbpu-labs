#ifndef TABLE_OPERATIONS_HPP
#define TABLE_OPERATIONS_HPP

#include <pqxx/pqxx>
#include <string>

namespace course {

void insert_into(pqxx::connection &c, const std::string &table_name,
                 const std::string &colomns, const std::string &values);
void delete_from(pqxx::connection &c, const std::string &table_name,
                 const std::string &where_cond);
void update_table(pqxx::connection &c, const std::string &table_name,
                  const std::string &column, const std::string &new_value,
                  const std::string &where_cond);
} // namespace course

#endif
