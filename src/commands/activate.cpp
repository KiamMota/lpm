#include "base.hpp"
#include "cli_base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace commands {
void activate(std::vector<std::string> &args) {

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

  std::string found_name;

  if (plugins_found.size() == 1) {
    found_name = std::filesystem::path(plugins_found.at(0)).stem().string();
  } else {
    std::cout << "multiple plugins found, pick one:" << std::endl;
    for (size_t i = 0; i < plugins_found.size(); i++) {
      std::string name =
          std::filesystem::path(plugins_found.at(i)).stem().string();
      std::cout << "  [" << i + 1 << "] " << name << std::endl;
    }
    std::cout << "> ";
    size_t choice;
    std::cin >> choice;
    if (choice < 1 || choice > plugins_found.size()) {
      std::cout << "invalid choice." << std::endl;
      return;
    }
    found_name =
        std::filesystem::path(plugins_found.at(choice - 1)).stem().string();
  }

  std::string require_line = "require('plugins." + found_name + "')";
  std::string commented_line = "-- " + require_line;

  // lê o init.lua
  std::ifstream file_in(base::nvPath.init_path);
  if (!file_in.is_open()) {
    std::cout << "err: failed to open init.lua" << std::endl;
    return;
  }
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file_in, line))
    lines.push_back(line);
  file_in.close();

  // procura se já existe (comentado ou não)
  bool found = false;
  for (std::string &l : lines) {
    if (l.find(require_line) != std::string::npos) {
      found = true;
      if (l.rfind("-- ", 0) == 0) {
        // está comentado, descomenta
        l = l.substr(3);
        std::cout << "activated: " << require_line << std::endl;
      } else {
        std::cout << "already active in init.lua." << std::endl;
        return;
      }
      break;
    }
  }

  // não encontrou nenhuma ocorrência, insere
  if (!found) {
    try {
      utils::insert_require_in_init_lua(require_line.c_str());
      std::cout << "done." << std::endl;
    } catch (const std::exception &ex) {
      std::cout << "err: " << ex.what() << std::endl;
    }
    return;
  }

  // reescreve o arquivo
  std::ofstream file_out(base::nvPath.init_path);
  if (!file_out.is_open()) {
    std::cout << "err: failed to write init.lua" << std::endl;
    return;
  }
  for (const std::string &l : lines)
    file_out << l << "\n";
  file_out.close();
}

} // namespace commands
