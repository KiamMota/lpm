#include "base.hpp"
#include "cli_base.hpp"
#include <commands.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace commands {
void cat(std::vector<std::string> &args) {
  if (args.empty()) {
    cli::require_arg("cat", "<plugin_name>");
    return;
  }
  std::string plugin_name = args.at(0);
  std::vector<std::string> plugins_found =
      utils::fuzzy_find_plugin(plugin_name.c_str());
  std::string chose_plugin = utils::when_find_multiple_plugins(plugins_found);
  if (chose_plugin.empty()) {
    std::cout << "not find." << std::endl;
    return;
  }
  if (!cli::msg_question("open '" + chose_plugin + "'?"))
    return;

  std::ifstream file(chose_plugin);
  if (!file.is_open()) {
    std::cout << "err: could not open " << chose_plugin << std::endl;
    return;
  }
  std::string line;
  while (std::getline(file, line)) {
    std::cout << line << std::endl;
  }

  file.close();
}

} // namespace commands
