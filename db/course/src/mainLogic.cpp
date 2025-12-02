#include "mainLogic.hpp"
#include <exception>
#include <iostream>
#include <pqxx/pqxx>

int course::mainLogic(int argc, char **argv) {
  try {
    pqxx::connection c;
    std::cout << "Connected to " << c.dbname() << '\n';
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
  }
  return 0;
}
