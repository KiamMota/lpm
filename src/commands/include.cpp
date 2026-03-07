#include "cli_base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace commands {
void include(std::vector<std::string> &args) {
  if (args.empty()) {
    cli::require_arg("include", "<plugin name>");
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

  // match exato: só um resultado
  if (plugins_found.size() == 1) {
    std::string found_name =
        std::filesystem::path(plugins_found.at(0)).stem().string();
    try {
      utils::insert_require_in_init_lua(
          ("require('plugins." + found_name + "')").c_str());
      std::cout << "done." << std::endl;
    } catch (const std::exception &ex) {
      std::cout << "err: " << ex.what() << std::endl;
    }
    return;
  }

  // múltiplos resultados: lista e pede pro usuário escolher
  std::cout << "multiple plugins found, pick one:" << std::endl;
  for (size_t i = 0; i < plugins_found.size(); i++) {
    std::string found_name =
        std::filesystem::path(plugins_found.at(i)).stem().string();
    std::cout << "  [" << i + 1 << "] " << found_name << std::endl;
  }

  std::cout << "> ";
  size_t choice;
  std::cin >> choice;

  if (choice < 1 || choice > plugins_found.size()) {
    std::cout << "invalid choice." << std::endl;
    return;
  }

  std::string chosen =
      std::filesystem::path(plugins_found.at(choice - 1)).stem().string();
  try {
    utils::insert_require_in_init_lua(
        ("require('plugins." + chosen + "')").c_str());
    std::cout << "done." << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "err: " << ex.what() << std::endl;
  }
}

} // namespace commands
