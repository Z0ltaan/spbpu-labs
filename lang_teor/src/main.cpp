#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "codegen.hpp"
#include "parser.hpp"

static void
print_help()
{
  std::cout << "Usage: cmilan <input_file>" << std::endl;
}

int
main(int argc, char** argv)
{
  if (argc != 2)
  {
    print_help();
    return -1;
  }

  try
  {
    std::ifstream input(argv[1]);

    if (!input.is_open())
    {
      std::string file_error_message = "File '";
      file_error_message += argv[1];
      file_error_message += "' not found";

      throw std::runtime_error(file_error_message);
    }

    cmilan::parser p(input);
    auto parsed_commands = p.parse();

    if (!p.error())
    {
      int address = 0;
      for (const auto& command: parsed_commands)
      {
        print_command(command, address++, std::cout);
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
}
