#include "base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

namespace commands {
namespace utils {
const std::string
when_find_multiple_plugins(std::vector<std::string> &plugins_found) {

  std::string chosen_path;

  if (plugins_found.empty())
    return {};
  if (plugins_found.size() == 1) {
    chosen_path = plugins_found.at(0);
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
      return {};
    }
    chosen_path = plugins_found.at(choice - 1);
  }
  return chosen_path;
}
void insert_require_in_init_lua(const std::string &content) {
  std::ifstream file_in(base::nvPath.init_path);
  if (!file_in.is_open())
    throw std::runtime_error("failed to open init.lua: " +
                             base::nvPath.init_path);

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file_in, line))
    lines.push_back(line);
  file_in.close();

  const std::vector<std::string> backup = lines;

  auto rollback = [&]() {
    std::ofstream restore(base::nvPath.init_path);
    if (!restore.is_open())
      return;
    for (const std::string &l : backup)
      restore << l << "\n";
    restore.close();
  };

  int lazy_line_index = -1;
  for (size_t i = 0; i < lines.size(); i++) {
    if (lines[i].find("require(config.lazy)") != std::string::npos) {
      lazy_line_index = static_cast<int>(i);
      std::cout << "lazy found!" << std::endl;
      break;
    }
  }

  if (lazy_line_index == -1) {
    std::cout << "warning: require(config.lazy) not found in init.lua, "
                 "inserting at end."
              << std::endl;
    lazy_line_index = static_cast<int>(lines.size()) - 1;
  }

  std::string new_line = content;
  for (const std::string &l : lines) {
    if (l == new_line)
      throw std::runtime_error("require already exists in init.lua: " +
                               new_line);
  }

  // insere a nova linha abaixo do require(config.lazy)
  lines.insert(lines.begin() + lazy_line_index + 1, new_line);

  // escreve o arquivo
  std::ofstream file_out(base::nvPath.init_path);
  if (!file_out.is_open()) {
    rollback();
    throw std::runtime_error("failed to open init.lua for writing: " +
                             base::nvPath.init_path);
  }

  for (const std::string &l : lines) {
    file_out << l << "\n";
    if (file_out.fail()) {
      file_out.close();
      rollback();
      throw std::runtime_error(
          "failed to write line to init.lua, rolled back.");
    }
  }

  file_out.close();
  if (file_out.fail()) {
    rollback();
    throw std::runtime_error("failed to close init.lua properly, rolled back.");
  }
}
const std::vector<std::string> fuzzy_find_plugin(const char *plugin_name) {
  if (!std::filesystem::exists(base::nvPath.plugins_path))
    throw std::runtime_error("plugins path not found: " +
                             base::nvPath.plugins_path);

  std::string target = plugin_name;
  std::vector<std::string> results;

  auto add_if_new = [&](const std::string &path) {
    if (std::find(results.begin(), results.end(), path) == results.end())
      results.push_back(path);
  };

  auto normalize = [](std::string name) -> std::string {
    for (const std::string suffix :
         {".nvim", "-nvim", ".lua", "-lua", ".vim", "-vim"}) {
      if (name.size() > suffix.size() &&
          name.substr(name.size() - suffix.size()) == suffix) {
        name = name.substr(0, name.size() - suffix.size());
      }
    }
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    return name;
  };

  std::string normalized_target = normalize(target);

  for (const auto &entry :
       std::filesystem::directory_iterator(base::nvPath.plugins_path)) {
    std::string filename = entry.path().stem().string();
    std::string normalized_filename = normalize(filename);
    std::string full_path = entry.path().string();

    // 1. exact match
    if (filename == target) {
      add_if_new(full_path);
      continue;
    }

    // 2. match normalizado
    if (normalized_filename == normalized_target) {
      add_if_new(full_path);
      continue;
    }

    // 3. fuzzy
    if (normalized_filename.find(normalized_target) != std::string::npos ||
        normalized_target.find(normalized_filename) != std::string::npos) {
      add_if_new(full_path);
      continue;
    }

    // 4. regex
    try {
      std::regex pattern(normalized_target, std::regex::icase);
      if (std::regex_search(normalized_filename, pattern))
        add_if_new(full_path);
    } catch (const std::regex_error &) {
    }
  }

  return results;
}
} // namespace utils

} // namespace commands
