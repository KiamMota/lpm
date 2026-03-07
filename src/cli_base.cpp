#include "cli_base.hpp"
#include "base.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace cli {

std::vector<std::string> main_args;

void help() {
  std::cout << "commands: \n\n"
            << "lazynv  install  <github url> <filename>\n"
            << "lazynv  remove   <filename>\n";
}

void version() { std::cout << VERSION_STR << std::endl; }

bool msg_question(const std::string &msg) {

  std::cout << msg << " [Y/n] ";
  std::string inp;
  std::getline(std::cin, inp);

  if (inp.empty() || inp == "y" || inp == "Y")
    return true;
  return false;
}

void require_arg(const char *cmd, const char *required_arg) {
  std::cout << cmd << " required another arg: " << required_arg << std::endl;
}

} // namespace cli
