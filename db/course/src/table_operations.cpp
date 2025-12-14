#include "table_operations.hpp"
#include <exception>
#include "utils/query.hpp"

extern bool show_error_window;
extern std::string last_error_message;

void
course::insert_into(pqxx::connection& c,
                    const std::string& table_name,
                    const std::string& colomns,
                    const std::string& values)
{
  std::string final_query("insert into " + table_name + " " + colomns +
                          " values " + values);
  try
  {
    pqxx::result res = query(c, final_query, {});
  }
  catch (const std::exception& e)
  {
    show_error_window = true;
    last_error_message = e.what();
  }
}

void
course::delete_from(pqxx::connection& c,
                    const std::string& table_name,
                    const std::string& where_cond)
{
  std::string final_query("delete from " + table_name + " where " + where_cond);
  try
  {
    pqxx::result res = query(c, final_query, {});
  }
  catch (const std::exception& e)
  {
    show_error_window = true;
    last_error_message = e.what();
  }
}

void
course::update_table(pqxx::connection& c,
                     const std::string& table_name,
                     const std::string& column,
                     const std::string& new_value,
                     const std::string& where_cond)
{
  std::string final_query("update " + table_name + " set " + column + " = " +
                          new_value + " where " + where_cond);
  try
  {
    pqxx::result res = query(c, final_query, {});
  }
  catch (const std::exception& e)
  {
    show_error_window = true;
    last_error_message = e.what();
  }
}
