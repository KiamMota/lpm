#include "cli_base.hpp"
#include "fsio.hpp"
#include <commands.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace commands {
void cat(std::vector<std::string> &args) {
  if (args.empty()) {
    cli::require_arg("cat", "<plugin_name>");
    return;
  }

  std::string plugin_name = args.at(0);
  std::vector<std::string> plugins_found;

  try {
    plugins_found = utils::fuzzy_find_plugin(plugin_name.c_str());
  } catch (const std::exception &ex) {
    std::cout << "err: " << ex.what() << std::endl;
    return;
  }

  if (plugins_found.empty()) {
    std::cout << "no plugin found for: " << plugin_name << std::endl;
    return;
  }

  std::string chosen_path = utils::when_find_multiple_plugins(plugins_found);
  if (chosen_path.empty())
    return;

  if (!cli::msg_question("open '" + chosen_path + "'?"))
    return;
  try {
    std::cout << fsio::read_file(chosen_path);
  } catch (const std::runtime_error &er) {
    std::cout << er.what() << std::endl;
  }
}

} // namespace commands
