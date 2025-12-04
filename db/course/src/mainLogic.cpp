#include "mainLogic.hpp"
#include "read_config.hpp"
#include "utils/query.hpp"
#include <iostream>
#include <pqxx/pqxx>
#include <string>

namespace course {
void insert_into(pqxx::connection &c, const std::string &table_name,
                 const std::string &colomns, const std::string &values) {
  std::string final_query("insert into " + table_name + " " + colomns +
                          " values " + values);
  pqxx::result res = query(c, final_query, {});
}

void delete_from(pqxx::connection &c, const std::string &table_name,
                 const std::string &where_cond) {
  std::string final_query("delete from " + table_name + " where " + where_cond);
  pqxx::result res = query(c, final_query, {});
}

void update_table(pqxx::connection &c, const std::string &table_name,
                  const std::string &column, const std::string &new_value,
                  const std::string &where_cond) {
  std::string final_query("update " + table_name + " set " + column + '=' +
                          new_value + " where " + where_cond);
}

void assign_work(pqxx::connection &c, const std::string &master,
                 const std::string &service, const std::string &car,
                 const std::string &date) {
  insert_into(c, "works", "(master_id, service_id, car_id, date_work)",
              '(' + master + ',' + service + ',' + car + ", '" + date + "')");
}
} // namespace course

int course::mainLogic(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: <program name> <config path>\n";
    return -1;
  }

  using query_res_t = pqxx::result;
  using row_t = pqxx::row;

  try {
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

    for (const auto &row : res) {
      auto [id, name] = row.as<int, std::string>();
      std::cout << id << ' ' << name << '\n';
    }

  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
  }
  return 0;
}
