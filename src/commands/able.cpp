#include "cli_base.hpp"
#include "commands.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

  std::ifstream file_in(chosen_path);
  if (!file_in.is_open()) {
    std::cout << "err: failed to open " << chosen_path << std::endl;
    return;
  }
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file_in, line))
    lines.push_back(line);
  file_in.close();

  const std::vector<std::string> backup = lines;

  bool found = false;
  for (std::string &l : lines) {
    if (l.find("enabled") == std::string::npos)
      continue;

    found = true;
    std::cout << "-- " << chosen_path << std::endl;
    if (l.find("true") != std::string::npos) {
      l.replace(l.find("true"), 4, "false");
      std::cout << "enabled = false" << std::endl;
    } else if (l.find("false") != std::string::npos) {
      l.replace(l.find("false"), 5, "true");
      std::cout << "enabled = true" << std::endl;
    }
    break;
  }

  if (!found) {
    // procura o último "}" e insere "enabled = true," antes dele
    int closing_brace_index = -1;
    for (int i = static_cast<int>(lines.size()) - 1; i >= 0; i--) {
      if (lines[i].find('}') != std::string::npos) {
        closing_brace_index = i;
        break;
      }
    }

    if (closing_brace_index == -1) {
      std::cout << "err: could not find closing brace in " << chosen_path
                << std::endl;
      return;
    }

    // descobre a indentação da linha anterior
    std::string indent = "  ";
    if (closing_brace_index > 0) {
      std::string prev = lines[closing_brace_index - 1];
      indent = "";
      for (char c : prev) {
        if (c == ' ' || c == '\t')
          indent += c;
        else
          break;
      }
      if (indent.empty())
        indent = "  ";
    }

    lines.insert(lines.begin() + closing_brace_index,
                 indent + "enabled = true,");

    std::cout << "enabled = true (inserted)" << std::endl;

    std::ofstream file_out(chosen_path);
    if (!file_out.is_open()) {
      std::cout << "err: failed to write " << chosen_path << std::endl;
      return;
    }
    for (const std::string &l : lines) {
      file_out << l << "\n";
      if (file_out.fail()) {
        file_out.close();
        std::ofstream restore(chosen_path);
        for (const std::string &bl : backup)
          restore << bl << "\n";
        std::cout << "err: write failed, rolled back." << std::endl;
        return;
      }
    }
    file_out.close();
    return;
  }
  std::ofstream file_out(chosen_path);
  if (!file_out.is_open()) {
    std::cout << "err: failed to write " << chosen_path << std::endl;
    return;
  }
  for (const std::string &l : lines) {
    file_out << l << "\n";
    if (file_out.fail()) {
      file_out.close();
      // rollback
      std::ofstream restore(chosen_path);
      for (const std::string &bl : backup)
        restore << bl << "\n";
      std::cout << "err: write failed, rolled back." << std::endl;
      return;
    }
  }
  file_out.close();
}

} // namespace commands
