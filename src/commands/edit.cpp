#include "cli_base.hpp"
#include "commands.hpp"
#include <cstdlib>
#include <iostream>

namespace commands {

void edit(std::vector<std::string> &args) {
  if (args.empty()) {
    cli::require_arg("edit", "<plugin name>");
    return;
  }

  bool use_nvim = false;
  std::vector<std::string> filtered_args;
  for (const std::string &arg : args) {
    if (arg == "--nvim")
      use_nvim = true;
    else
      filtered_args.push_back(arg);
  }

  if (filtered_args.empty()) {
    cli::require_arg("edit", "<plugin name>");
    return;
  }

  std::string plugin_name = filtered_args.at(0);
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

  std::string editor;
  if (use_nvim) {
    editor = "nvim";
  } else {
    const char *env_editor = std::getenv("EDITOR");
    editor = (env_editor != nullptr) ? env_editor : "nvim";
  }

  std::string cmd = editor + " \"" + chosen_path + "\"";
  [[maybe_unused]] int result = std::system(cmd.c_str());
}

} // namespace commands
