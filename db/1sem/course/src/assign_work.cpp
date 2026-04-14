#include "assign_work.hpp"
#include "table_operations.hpp"

void
course::assign_work(pqxx::connection& c,
                    const std::string& master,
                    const std::string& service,
                    const std::string& car,
                    const std::string& date)
{
  insert_into(c,
              "works",
              "(master_id, service_id, car_id, date_work)",
              '(' + master + ',' + service + ',' + car + ", '" + date + "')");
}
