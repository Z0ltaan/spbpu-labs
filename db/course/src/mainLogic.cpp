#include "mainLogic.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <pqxx/pqxx>

#include <string>
#include "assign_work.hpp"
#include "gui/clickable_table.hpp"
#include "read_config.hpp"
#include "table_operations.hpp"
#include "utils/query.hpp"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

int
course::mainLogic(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: <program name> <config path>\n";
    return -1;
  }

  using query_res_t = pqxx::result;
  using row_t = pqxx::row;

  try
  {
    // NOTE: create connection to database
    std::string connection_options;
    read_config(connection_options, argv[1]);
    pqxx::connection connection(connection_options);
    std::cout << "Connected to " << connection.dbname() << '\n';

    // pqxx::result res = query(connection, "call get_scores_table($1)", {2});
    // auto arr_entry = res.front().front().as_sql_array<std::string>();

    // pqxx::result res = query(connection, "call sum_up_costs($1)",
    // {"10months"}); auto arr_entry = res.front().front().as<int>();
    // std::cout << arr_entry << '\n';
    delete_from(connection, "masters", "name='van'");
    insert_into(connection, "masters", "(name)", "('van')");
    query_res_t res =
      query(connection, "SELECT * from masters where name = 'van'", {});

    assign_work(connection, "11", "1", "1", "2020-04-12");

    for (const auto& row: res)
    {
      auto [id, name] = row.as< int, std::string >();
      std::cout << id << ' ' << name << '\n';
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
  }
  return 0;
}
