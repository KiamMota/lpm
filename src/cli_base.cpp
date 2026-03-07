#include "cli_base.hpp"
#include "base.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace cli {

std::vector<std::string> main_args;

void help() {
  std::cout << "\nlazynvimcli - a lazy plugin manager cli\n\n"
            << "usage:\n"
            << "  lazynvimcli <command> [arguments]\n\n"
            << "commands:\n"
            << "  install  <github url>   install a plugin from github\n"
            << "  remove   <plugin name>  remove an installed plugin\n"
            << "  include  <plugin name>  add require to init.lua\n"
            << "  list                    list installed plugins\n\n"
            << "flags:\n"
            << "  -v, --version           show version\n"
            << std::endl;
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
