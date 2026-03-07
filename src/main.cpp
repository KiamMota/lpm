#include "base.hpp"
#include "cli_base.hpp"
#include "commands.hpp"
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

int main(int argn, char **argv) {
  if (argn == 1) {
    cli::help();
    return 0;
  }

  for (int i = 2; i < argn; i++)
    cli::main_args.push_back(argv[i]);

  base::nvPath.base_path = base::get_nvim_config_path();
  base::nvPath.init_path = base::nvPath.base_path + "/init.lua";
  base::nvPath.lua_path = base::nvPath.base_path + "/lua";
  base::nvPath.config_path = base::nvPath.base_path + "/lua/config";
  base::nvPath.plugins_path = base::nvPath.base_path + "/lua/plugins";

  const std::unordered_map<std::string, std::function<void()>> simple_commands =
      {
          {"--version", []() { cli::version(); }},
          {"-v", []() { cli::version(); }},
          {"list", []() { commands::list(); }},
          {"--help", []() { cli::help(); }},
          {"-h", []() { cli::help(); }},
          {"--self-update", []() { commands::self_update(); }},
      };

  const std::unordered_map<std::string,
                           std::function<void(std::vector<std::string> &)>>
      arg_commands = {
          {"install", commands::install},
          {"remove", commands::remove},
          {"activate", commands::activate},
          {"deactivate", commands::deactivate},
      };

  std::string cmd = argv[1];

  if (auto it = simple_commands.find(cmd); it != simple_commands.end()) {
    it->second();
    return 0;
  }

  if (auto it = arg_commands.find(cmd); it != arg_commands.end()) {
    it->second(cli::main_args);
    return 0;
  }

  std::cout << "unknown command: '" << cmd << "'\n" << std::endl;
  cli::help();
  return 1;
}
