#include "read_config.hpp"
#include <fstream>

void course::read_config(std::string &dest, const char *config_path) {
  std::ifstream in(config_path);
  if (!in.is_open()) {
    throw std::runtime_error("Can not open configuration file");
  }

  std::string temp;
  while (std::getline(in, temp)) {
    if (!temp.empty()) {
      dest += (temp += " ");
    }
  };

  dest.erase(dest.find_last_not_of(' ') + 1);
}
