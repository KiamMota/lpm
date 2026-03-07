#include "cli_base.hpp"
#include "base.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace cli {

std::vector<std::string> main_args;

void help() {
  std::cout
      << "\nlpm — lazy plugin manager\n\n"
      << "usage:  lpm <command> [argument]\n\n"
      << "commands:\n"
      << "  install    <github url>   download and create plugin config\n"
      << "  remove     <plugin name>  delete plugin config file\n"
      << "  activate   <plugin name>  uncomment require in init.lua\n"
      << "  deactivate <plugin name>  comment out require in init.lua\n"
      << "  list                      list installed plugins\n\n"
      << "flags:\n"
      << "  -v, --version             show version\n\n"
      << "examples:\n"
      << "  lpm install https://github.com/nvim-telescope/telescope.nvim\n"
      << "  lpm remove telescope\n"
      << "  lpm deactivate telescope\n"
      << "  lpm activate telescope\n"
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
