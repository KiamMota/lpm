#include "base.hpp"
#include "cli_base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
namespace commands {
void remove(std::vector<std::string> &commands) {
  if (commands.empty()) {
    cli::require_arg("remove", "<plugin name>");
    return;
  }

  std::string plugin_to_delete = commands.at(0);

  // fuzzy find
  std::vector<std::string> plugins_found;
  try {
    plugins_found = utils::fuzzy_find_plugin(plugin_to_delete.c_str());
  } catch (const std::exception &ex) {
    std::cout << "err: " << ex.what() << std::endl;
    return;
  }

  if (plugins_found.empty()) {
    std::cout << "no plugin found for: " << plugin_to_delete << std::endl;
    return;
  }

  const std::string chosen_path =
      utils::when_find_multiple_plugins(plugins_found);
  std::string plugin_name = std::filesystem::path(chosen_path).stem().string();

  if (!cli::msg_question("Remove '" + plugin_name + "'?"))
    return;

  // backup do init.lua para rollback
  std::ifstream file_in(base::nvPath.init_path);
  if (!file_in.is_open()) {
    std::cout << "failed to open init.lua." << std::endl;
    return;
  }
  std::stringstream buffer;
  buffer << file_in.rdbuf();
  file_in.close();
  const std::string backup_content = buffer.str();
  // remove o arquivo .lua do plugin
  try {
    std::filesystem::remove(chosen_path);
    std::cout << "removed: " << plugin_name << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "err removing file: " << ex.what() << std::endl;
    return;
  }

  // remove o require do init.lua
  std::string content = backup_content;
  std::string require_line = "require('plugins." + plugin_name + "')";
  size_t pos = content.find(require_line);

  if (pos != std::string::npos) {
    size_t line_start = content.rfind('\n', pos);
    line_start = (line_start == std::string::npos) ? 0 : line_start;
    size_t line_end = content.find('\n', pos);
    line_end = (line_end == std::string::npos) ? content.size() : line_end + 1;
    content.erase(line_start, line_end - line_start);

    std::ofstream file_out(base::nvPath.init_path);
    if (!file_out.is_open()) {
      // rollback: restaura o arquivo do plugin
      std::cout << "failed to open init.lua for writing, rolling back..."
                << std::endl;
      std::filesystem::copy(chosen_path, chosen_path); // já foi removido, avisa
      std::cout << "warning: plugin file was already removed, could not fully "
                   "rollback."
                << std::endl;
      return;
    }

    file_out << content;
    if (file_out.fail()) {
      file_out.close();
      // rollback do init.lua
      std::ofstream restore(base::nvPath.init_path);
      restore << backup_content;
      restore.close();
      std::cout << "failed to write init.lua, rolled back." << std::endl;
      return;
    }

    file_out.close();
    std::cout << "removed require from init.lua." << std::endl;
  } else {
    std::cout << "require not found in init.lua, skipping." << std::endl;
  }
}
} // namespace commands
