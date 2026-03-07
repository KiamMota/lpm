#include "base.hpp"
#include "cli_base.hpp"
#include "commands.hpp"
#include <cstring>
#include <vector>

int main(int argn, char **argv) {
  if (argn == 1) {
    cli::help();
    return 0;
  }

  for (int i = 2; i < argn; i++) {
    cli::main_args.push_back(argv[i]);
  }

  base::nvPath.base_path = base::get_nvim_config_path();
  base::nvPath.init_path = base::nvPath.base_path + "/init.lua";
  base::nvPath.lua_path = base::nvPath.base_path + "/lua";
  base::nvPath.config_path = base::nvPath.base_path + "/lua/config";
  base::nvPath.plugins_path = base::nvPath.base_path + "/lua/plugins";

  if (argn >= 2) {
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
      cli::version();
      return 0;
    }
    if (strcmp(argv[1], "install") == 0) {
      commands::install(cli::main_args);
      return 0;
    }
    if (strcmp(argv[1], "list") == 0) {
      commands::list();
      return 0;
    }
    if (strcmp(argv[1], "remove") == 0) {
      commands::remove(cli::main_args);
      return 0;
    }

    if (strcmp(argv[1], "activate") == 0) {
      commands::activate(cli::main_args);
      return 0;
    }

    if (strcmp(argv[1], "deactivate") == 0) {
      commands::deactivate(cli::main_args);
      return 0;
    }
  }
  return 1;
}
