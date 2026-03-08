#include "cli_base.hpp"
#include "commands.hpp"
#include "fsio.hpp"
#include <iostream>
#include <string>
#include <vector>

static bool toggle_enabled(std::vector<std::string> &lines) {
  for (std::string &l : lines) {
    if (l.find("enabled") == std::string::npos)
      continue;
    if (l.find("true") != std::string::npos) {
      l.replace(l.find("true"), 4, "false");
      std::cout << "enabled = false" << std::endl;
    } else if (l.find("false") != std::string::npos) {
      l.replace(l.find("false"), 5, "true");
      std::cout << "enabled = true" << std::endl;
    }
    return true;
  }
  return false;
}
static void insert_enabled(std::vector<std::string> &lines) {
  int idx = fsio::find_last_closing_brace(lines);
  if (idx == -1)
    throw std::runtime_error("could not find closing brace");
  std::string indent = fsio::get_indent(lines, idx);
  lines.insert(lines.begin() + idx, indent + "enabled = true,");
  std::cout << "enabled = true (inserted)" << std::endl;
}
namespace commands {

void able(std::vector<std::string> &args) {
  if (args.empty()) {
    cli::require_arg("able", "<plugin name>");
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

  std::vector<std::string> lines;
  try {
    lines = fsio::read_lines(chosen_path);
  } catch (const std::exception &ex) {
    std::cout << "err: " << ex.what() << std::endl;
    return;
  }

  const std::vector<std::string> backup = lines;

  try {
    std::cout << "-- " << chosen_path << std::endl;
    if (!toggle_enabled(lines))
      insert_enabled(lines);
    fsio::write_lines(chosen_path, lines);
  } catch (const std::exception &ex) {
    std::cout << "err: " << ex.what() << ", rolling back..." << std::endl;
    try {
      fsio::write_lines(chosen_path, backup);
    } catch (...) {
    }
  }
}

} // namespace commands
