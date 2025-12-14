#ifndef ASSIGN_WORK_HPP
#define ASSIGN_WORK_HPP

#include <pqxx/pqxx>

namespace course
{
  void assign_work(pqxx::connection& c,
                   const std::string& master,
                   const std::string& service,
                   const std::string& car,
                   const std::string& date);
}

#endif
